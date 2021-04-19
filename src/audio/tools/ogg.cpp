#include "ogg.hpp"

#include <cstring>
#include <iostream>
#include <limits>
#include <vector>
#include "../../utils/assert.hpp"
#include "../../utils/messages.hpp"

namespace Zen {

static std::size_t read_ogg_callback (
		void *buffer,
		std::size_t elementCount,
		std::size_t elementSize,
		void *dataSource)
{
	ZEN_ASSERT((elementCount == 1), "Function: static std::size_t read_ogg_callback (void *buffer, std::size_t elementSize, std::size_t elementCount, void *dataSource)");

	std::ifstream& stream = *static_cast<std::ifstream*>(dataSource);
	stream.read(static_cast<char*>(buffer), elementSize);
	const std::streamsize bytesRead = stream.gcount();
	stream.clear(); // In case we read past EOF
	return static_cast<size_t>(bytesRead);
}

static std::int32_t seek_ogg_callback (void *dataSource, ogg_int64_t offset, std::int32_t origin)
{
	static const std::vector<std::ios_base::seekdir> seekDirections {
		std::ios_base::beg,
		std::ios_base::cur,
		std::ios_base::end
	};

	std::ifstream& stream = *reinterpret_cast<std::ifstream*>(dataSource);
	stream.seekg(offset, seekDirections.at(origin));
	stream.clear(); // In case we seeked to EOF
	return 0;
}

static long int tell_ogg_callback (void *dataSource)
{
	std::ifstream& stream = *reinterpret_cast<std::ifstream*>(dataSource);
	const auto position = stream.tellg();
	ZEN_ASSERT((position >= 0), "Function: static long int tell_ogg_callback (void *dataSource)");
	return static_cast<long>(position);
}

static std::size_t read_ogg_stream_callback (
		void *destination,
		std::size_t size1,
		std::size_t size2,
		void *fileHandle)
{
	AudioStreamData *audioData = reinterpret_cast<AudioStreamData*>(fileHandle);

	ALsizei length = size1 * size2;

	if (audioData->sizeConsumed + length > audioData->size)
		length = audioData->size - audioData->sizeConsumed;

	if (!audioData->file.is_open())
	{
		audioData->file.open(audioData->filename, std::ios::binary);
		if (!audioData->file.is_open())
		{
			MessageError("Could not re-open streaming file\"", audioData->filename, "\"");
			return 0;
		}
	}

	std::vector<char> moreData;
	moreData.reserve(length);
	moreData.resize(length);

	audioData->file.clear();
	audioData->file.seekg(audioData->sizeConsumed);
	if (!audioData->file.read(&moreData[0], length))
	{
		if (audioData->file.eof())
		{
			audioData->file.clear(); // just clear the error, it's resolved later
		}
		else if (audioData->file.fail())
		{
			MessageError("OGG stream has fail bit set ", audioData->filename);
			//audioData->file.close();
			return 0;
		}
		else if (audioData->file.bad())
		{
			MessageError("OGG stream has bad bit set ", audioData->filename);
			//audioData->file.close();
			return 0;
		}
	}

	audioData->sizeConsumed += length;

	std::memcpy(destination, &moreData[0], length);

	audioData->file.clear();

	return length;
}

static std::int32_t seek_ogg_stream_callback (void *fileHandle, ogg_int64_t to, std::int32_t type)
{
	AudioStreamData *audioData = reinterpret_cast<AudioStreamData*>(fileHandle);

	if (type == SEEK_CUR)
		audioData->sizeConsumed += to;
	else if (type == SEEK_END)
		audioData->sizeConsumed = audioData->size - to;
	else if (type == SEEK_SET)
		audioData->sizeConsumed = to;
	else
		return -1; // What are you trying to do Vorbis?

	if (audioData->sizeConsumed < 0)
	{
		audioData->sizeConsumed = 0;
		return -1;
	}
	
	if (audioData->sizeConsumed > audioData->size)
	{
		audioData->sizeConsumed = audioData->size;
		return -1;
	}

	return 0;
}

static long int tell_ogg_stream_callback (void *fileHandle)
{
	AudioStreamData *audioData = reinterpret_cast<AudioStreamData*>(fileHandle);

	return audioData->sizeConsumed;
}

int setup_stream_ogg (const std::string& filename, AudioStreamData *audioStream)
{
	audioStream->filename = filename;
	audioStream->file.open(filename, std::ios::binary);
	if (!audioStream->file.is_open())
	{
		MessageError("Couldn't open file \"", filename, "\"");
		return -1;
	}

	audioStream->file.seekg(0, std::ios_base::beg);
	audioStream->file.ignore(std::numeric_limits<std::streamsize>::max());
	audioStream->size = audioStream->file.gcount();

	audioStream->file.clear();
	audioStream->file.seekg(0, std::ios_base::beg);
	audioStream->sizeConsumed = 0;

	ov_callbacks oggCallbacks {
		.read_func = read_ogg_stream_callback,
		.seek_func = seek_ogg_stream_callback,
		.close_func = nullptr,
		.tell_func = tell_ogg_stream_callback
	};

	if (ov_open_callbacks(reinterpret_cast<void*>(audioStream), &audioStream->oggVorbisFile, nullptr, -1, oggCallbacks) < 0)
	{
		MessageError("Could not ov_open_callbacks");
		return -1;
	}

	vorbis_info* vorbisInfo = ov_info(&audioStream->oggVorbisFile, -1);

	audioStream->channels = vorbisInfo->channels;
	audioStream->bitsPerSample = 16;
	audioStream->sampleRate = vorbisInfo->rate;
	audioStream->duration = ov_time_total(&audioStream->oggVorbisFile, -1);

	ZEN_AL_CALL(alGenBuffers, ZEN_AUDIO_STREAM_BUFFERS_NUM, &audioStream->buffers[0]);

	if (audioStream->file.eof()) {
		MessageError("Already reached EOF without loading data");
		return false;
	} else if (audioStream->file.fail()) {
		MessageError("Fail bit set");
		return false;
	} else if (!audioStream->file) {
		MessageError("File is false");
		return false;
	}

	std::array<char, ZEN_AUDIO_STREAM_BUFFERS_SIZE> data;

	for (std::uint8_t i = 0; i < ZEN_AUDIO_STREAM_BUFFERS_NUM; i++)
	{
		std::int32_t dataSoFar = 0;

		while (dataSoFar < ZEN_AUDIO_STREAM_BUFFERS_SIZE)
		{
			std::int32_t result = ov_read(
					&audioStream->oggVorbisFile,
					&data[dataSoFar],
					ZEN_AUDIO_STREAM_BUFFERS_SIZE - dataSoFar,
					0, 2, 1,
					&audioStream->oggCurrentSection);

			if (result == OV_HOLE) {
				MessageError("OV_HOLE found in initial read of buffer ", (int)i);
				break;
			}
			else if (result == OV_EBADLINK) {
				MessageError("OV_EBADLINK found in initial read of buffer ", (int)i);
				break;
			}
			else if (result == OV_EINVAL) {
				MessageError("OV_EINVAL found in initial read of buffer ", (int)i);
				MessageNote("Make sure the buffer size is a multiple of 4 (Preferably 4096)");
				break;
			}
			else if (result == 0) {
				MessageError("EOF found in initial read of buffer ", (int)i);
				break;
			}

			dataSoFar += result;
		}

		if (audioStream->channels == 1 && audioStream->bitsPerSample == 8)
			audioStream->format = AL_FORMAT_MONO8;
		else if (audioStream->channels == 1 && audioStream->bitsPerSample == 16)
			audioStream->format = AL_FORMAT_MONO16;
		else if (audioStream->channels == 2 && audioStream->bitsPerSample == 8)
			audioStream->format = AL_FORMAT_STEREO8;
		else if (audioStream->channels == 2 && audioStream->bitsPerSample == 16)
			audioStream->format = AL_FORMAT_STEREO16;
		else {
			MessageError("Unrecognized ogg format: ",
					audioStream->channels, " channels, ",
					audioStream->bitsPerSample, " bps");
			return -1;
		}

		ZEN_AL_CALL(alBufferData, audioStream->buffers[i], audioStream->format, data.data(), dataSoFar, audioStream->sampleRate);
	}

	ZEN_AL_CALL(alSourceQueueBuffers, audioStream->source, ZEN_AUDIO_STREAM_BUFFERS_NUM, &audioStream->buffers[0]);

	return 0;
}

int update_stream_ogg (AudioStreamData *audioStream)
{
	ALint buffersProcessed = 0;
	ZEN_AL_CALL(alGetSourcei, audioStream->source, AL_BUFFERS_PROCESSED, &buffersProcessed);

	ALint state;
	ZEN_AL_CALL(alGetSourcei, audioStream->source, AL_SOURCE_STATE, &state);

	if (buffersProcessed <= 0 || state != AL_PLAYING)
		return 0;

	int retValue = 0;

	while (buffersProcessed--)
	{
		std::array<char, ZEN_AUDIO_STREAM_BUFFERS_SIZE> data;
		std::memset(data.data(), 0, ZEN_AUDIO_STREAM_BUFFERS_SIZE);

		ALsizei dataSizeToBuffer = 0;
		std::int32_t sizeRead = 0;

		while (sizeRead < ZEN_AUDIO_STREAM_BUFFERS_SIZE)
		{
			std::int32_t result = ov_read(
					&audioStream->oggVorbisFile,
					&data[sizeRead],
					ZEN_AUDIO_STREAM_BUFFERS_SIZE - sizeRead,
					0, 2, 1,
					&audioStream->oggCurrentSection);

			if (result == OV_HOLE) {
				MessageError("OV_HOLE found in update of buffer");
				break;
			}
			else if (result == OV_EBADLINK) {
				MessageError("OV_EBADLINK found in update of buffer");
				break;
			}
			else if (result == OV_EINVAL) {
				MessageError("OV_EINVAL found in update of buffer");
				break;
			}
			else if (result == 0) {
				// End of file
				// Loop from the beginning
				if (audioStream->loop)
				{
					std::int32_t seekResult =
						ov_raw_seek(&audioStream->oggVorbisFile, 0);

					retValue = 2;

					switch (seekResult)
					{
						case OV_ENOSEEK:
							MessageError("OV_ENOSEEK found when trying to loop");
							break;
						case OV_EINVAL:
							MessageError("OV_EINVAL found when trying to loop");
							break;
						case OV_EREAD:
							MessageError("OV_EREAD found when trying to loop");
							break;
						case OV_EFAULT:
							MessageError("OV_EFAULT found when trying to loop");
							break;
						case OV_EOF:
							MessageError("OV_EOF found when trying to loop");
							break;
						case OV_EBADLINK:
							MessageError("OV_EBADLINK found when trying to loop");
							break;

						if (seekResult != 0) {
							MessageError("Unknown error in ov_raw_seek");
							return -1;
						}
					}
				}
				// Don't add any data
				else
				{
					sizeRead = 0;
					retValue = 1;
					break;
				}
			}

			sizeRead += result;
		}

		dataSizeToBuffer = sizeRead;

		if (dataSizeToBuffer > 0)
		{
			// Unqueue buffer
			ALuint buffer;
			ZEN_AL_CALL(alSourceUnqueueBuffers, audioStream->source, 1, &buffer);

			// Write data to it
			ZEN_AL_CALL(alBufferData, buffer, audioStream->format, data.data(), dataSizeToBuffer, audioStream->sampleRate);

			// Requeue it to the front, making UNPROCESSED again
			ZEN_AL_CALL(alSourceQueueBuffers, audioStream->source, 1, &buffer);
		}
	}

	return retValue;
}

int load_ogg (const std::string& filename, AudioBuffer *audioBuffer)
{
	audioBuffer->filename = filename;

	std::ifstream audioFile(filename, std::ios::binary);
	if (!audioFile.is_open())
	{
		std::cerr << "ERROR: Couldn't open file \"" << filename << "\"" << std::endl;
		return -1;
	}

	audioFile.seekg(0, std::ios_base::beg);
	audioFile.ignore(std::numeric_limits<std::streamsize>::max());
	ALsizei size = audioFile.gcount();

	audioFile.clear();
	audioFile.seekg(0, std::ios_base::beg);

	ov_callbacks oggCallbacks {
		.read_func = read_ogg_callback,
		.seek_func = seek_ogg_callback,
		.close_func = nullptr,
		.tell_func = tell_ogg_callback
	};

	OggVorbis_File oggVorbisFile;
	int oggCurrentSection = 0;

	if ( ov_open_callbacks(reinterpret_cast<void*>(&audioFile), &oggVorbisFile, nullptr, -1, oggCallbacks) < 0 )
	{
		std::cerr << "ERROR: Could not ov_open_callbacks" << std::endl;
		return -1;
	}

	vorbis_info* vorbisInfo = ov_info(&oggVorbisFile, -1);

	std::uint8_t channels = vorbisInfo->channels;
	std::uint8_t bitsPerSample = 16;
	std::uint32_t sampleRate = vorbisInfo->rate;
	double duration = ov_time_total(&oggVorbisFile, -1);

	ZEN_AL_CALL(alGenBuffers, 1, &audioBuffer->buffer);

	if (audioFile.eof())
	{
		std::cerr << "ERROR: Already reached EOF without loading data" << std::endl;
		return -1;
	}
	else if (audioFile.fail())
	{
		std::cerr << "ERROR: Fail bit set" << std::endl;
		return -1;
	}
	else if (!audioFile)
	{
		std::cerr << "ERROR: File is false" << std::endl;
		return -1;
	}

	std::size_t rawSize = channels * (bitsPerSample/8.) * sampleRate * duration;

	// FIXME Without this horror, the file ends a bit too early
	rawSize += 3000;

	rawSize -= rawSize%4;

	std::vector<char> data;
	data.reserve(rawSize);
	data.resize(rawSize);
	std::memset(&data[0], 0, rawSize);

	std::size_t dataSoFar = 0;
	std::int32_t result = -1;

	while (result != 0)
	{
		result = ov_read(&oggVorbisFile, &data[dataSoFar], rawSize - dataSoFar, 0, 2, 1, &oggCurrentSection);

		if (result == OV_HOLE) {
			std::cerr
				<< "ERROR: OV_HOLE found in initial read of buffer "
				<< std::endl;
				break;
		}
		else if (result == OV_EBADLINK) {
			std::cerr
				<< "ERROR: OV_EBADLINK found in initial read of buffer "
				<< std::endl;
			break;
		}
		else if (result == OV_EINVAL) {
			std::cerr
				<< "ERROR: OV_EINVAL found in initial read of buffer "
				<< std::endl;
			break;
		}

		dataSoFar += result;
	}

	ov_clear(&oggVorbisFile);
	audioFile.close();

	ALenum format;
	if (channels == 1 && bitsPerSample == 8)
		format = AL_FORMAT_MONO8;
	else if (channels == 1 && bitsPerSample == 16)
		format = AL_FORMAT_MONO16;
	else if (channels == 2 && bitsPerSample == 8)
		format = AL_FORMAT_STEREO8;
	else if (channels == 2 && bitsPerSample == 16)
		format = AL_FORMAT_STEREO16;
	else {
		std::cerr
			<< "ERROR: Unrecognized ogg format: "
			<< channels << " channels, "
			<< bitsPerSample << " bps"
			<< std::endl;
		return -1;
	}

	// Copy data into the OpenAL buffer
	ZEN_AL_CALL(alBufferData, audioBuffer->buffer, format, data.data(), rawSize, sampleRate);

	return 0;
}

int rewind_stream_ogg (AudioStreamData *audioStream)
{
	// Stop the source
	ZEN_AL_CALL(alSourceStop, audioStream->source);

	// Rewind the OGG file
	std::int32_t seekResult =
		ov_raw_seek(&audioStream->oggVorbisFile, 0);

	switch (seekResult)
	{
		case OV_ENOSEEK:
			MessageError("OV_ENOSEEK found when trying to rewind");
			break;
		case OV_EINVAL:
			MessageError("OV_EINVAL found when trying to rewind");
			break;
		case OV_EREAD:
			MessageError("OV_EREAD found when trying to rewind");
			break;
		case OV_EFAULT:
			MessageError("OV_EFAULT found when trying to rewind");
			break;
		case OV_EOF:
			MessageError("OV_EOF found when trying to rewind");
			break;
		case OV_EBADLINK:
			MessageError("OV_EBADLINK found when trying to rewind");
			break;

		if (seekResult != 0) {
			MessageError("Unknown error in ov_raw_seek");
			return -1;
		}
	}

	update_stream_ogg(audioStream);

	return 0;
}

void close_stream_ogg (AudioStreamData *audioData)
{
	ov_clear(&audioData->oggVorbisFile);

	ZEN_AL_CALL(alDeleteSources, 1, &audioData->source);

	ZEN_AL_CALL(alDeleteBuffers, ZEN_AUDIO_STREAM_BUFFERS_NUM, &audioData->buffers[0]);

	audioData->file.close();
}

}	// namespace Zen
