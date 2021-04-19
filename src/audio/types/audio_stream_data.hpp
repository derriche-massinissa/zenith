/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_AUDIO_TYPES_AUDIOSTREAMDATA_HPP
#define ZEN_AUDIO_TYPES_AUDIOSTREAMDATA_HPP

#define ZEN_AUDIO_STREAM_BUFFERS_NUM 4
#define ZEN_AUDIO_STREAM_BUFFERS_SIZE 4096
//#define ZEN_AUDIO_STREAM_BUFFERS_SIZE 65536

#include <array>
#include <string>
#include <fstream>
#include "../tools/al_utility.hpp"
//#include "../../utils/messages.hpp"
#include <vorbis/vorbisfile.h>

namespace Zen {

struct AudioStreamData
{
	/**
	 * @property source this is the OpenAL Source that is generated to play the
	 * file. You set the gain and position of the file with this.
	 */
	ALuint source;

	/**
	 * @property buffers are loaded into as the previous one is playing. That’s
	 * how the streaming works, one buffer is handed off to OpenAL as the
	 * previous ones are having the next few milliseconds of audio provided.
	 */
	std::array<ALuint, ZEN_AUDIO_STREAM_BUFFERS_NUM> buffers;

	/**
	 * @property filename is the filename of the original .ogg file. This is used
	 * in case the file handle is lost, to continue streaming we need to know
	 * which file to re-open.
	 */
	std::string filename;

	/**
	 * @property file is the file handle.
	 */
	std::ifstream file;

	/**
	 * @property channels
	 */
	std::uint8_t channels;

	/**
	 * @property sampleRate
	 */
	std::uint32_t sampleRate;

	/**
	 * @property bitsPerSample
	 */
	std::uint8_t bitsPerSample;

	/**
	 * @property size this is the total size of the ogg file in bytes. Populated
	 * when the file is first loaded.
	 */
	ALsizei size;

	/**
	 * @property sizeConsumed is used by the callbacks to understand where in the
	 * audio data that playback is up to.
	 */
	ALsizei sizeConsumed = 0;

	/**
	 * @property format is the OpenAL enum of either MONO or STEREO
	 */
	ALenum format;

	/**
	 * @property oggVoribsFile is a handle needed by Ogg Vorbis library.
	 */
	OggVorbis_File oggVorbisFile;

	/**
	 * @property oggCurrentSection is the part of the audio that’s currently
	 * being played back.
	 */
	int oggCurrentSection = 0;

	/**
	 * @property duration is the total length of the audio data.
	 */
	std::size_t duration;

	/**
	 * @property loop indicates if the stream should loop from the begining when
	 * done.
	 */
	bool loop = false;

	/*
	AudioStream ()
	{}

	AudioStream (const AudioStream& other) = delete;

	AudioStream (AudioStream&& other)
		: source (other.source)
		, buffers (other.buffers)
		, filename (other.filename)
		, file (std::move(other.file))
		, channels (other.channels)
		, sampleRate (other.sampleRate)
		, bitsPerSample (other.bitsPerSample)
		, size (other.size)
		, sizeConsumed (other.sizeConsumed)
		, format (other.format)
		, oggVorbisFile (other.oggVorbisFile)
		, oggCurrentSection (other.oggCurrentSection)
		, duration (other.duration)
		, loop (other.loop)
	{
		oggVorbisFile.datasource = reinterpret_cast<void*>(this);
	}

	void operator = (const AudioStream& other) = delete;

	void operator = (AudioStream&& other)
	{
		source = other.source;
		buffers = other.buffers;
		filename = other.filename;
		file = std::move(other.file);
		channels = other.channels;
		sampleRate = other.sampleRate;
		bitsPerSample = other.bitsPerSample;
		size = other.size;
		sizeConsumed = other.sizeConsumed;
		format = other.format;
		oggVorbisFile = other.oggVorbisFile;
		oggCurrentSection = other.oggCurrentSection;
		duration = other.duration;
		loop = other.loop;
	}

	~AudioStreamData ()
	{
		ZEN_AL_CALL(alDeleteSources, 1, &source);
		ZEN_AL_CALL(alDeleteBuffers, ZEN_AUDIO_STREAM_BUFFERS_NUM, &buffers[0]);
		file.close();
	}
	*/
};

}	// namespace Zen

#endif
