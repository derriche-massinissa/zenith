/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_AUDIO_TYPES_AUDIOSTREAMDATA_HPP
#define ZEN_AUDIO_TYPES_AUDIOSTREAMDATA_HPP

#define ZEN_AUDIO_STREAM_BUFFERS_NUM 4
#define ZEN_AUDIO_STREAM_BUFFERS_SIZE 65536 //4096
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
	 * @since 0.0.0
	 *
	 * @property source this is the OpenAL Source that is generated to play the
	 * file. You set the gain and position of the file with this.
	 */
	ALuint source;

	/**
	 * @since 0.0.0
	 *
	 * @property buffers are loaded into as the previous one is playing. That’s
	 * how the streaming works, one buffer is handed off to OpenAL as the
	 * previous ones are having the next few milliseconds of audio provided.
	 */
	std::array<ALuint, ZEN_AUDIO_STREAM_BUFFERS_NUM> buffers;

	/**
	 * @since 0.0.0
	 *
	 * @property filename is the filename of the original .ogg file. This is used
	 * in case the file handle is lost, to continue streaming we need to know
	 * which file to re-open.
	 */
	std::string filename;

	/**
	 * @since 0.0.0
	 *
	 * @property file is the file handle.
	 */
	std::ifstream file;

	/**
	 * @since 0.0.0
	 *
	 * @property channels
	 */
	std::uint8_t channels;

	/**
	 * @since 0.0.0
	 *
	 * @property sampleRate
	 */
	std::uint32_t sampleRate;

	/**
	 * @since 0.0.0
	 *
	 * @property bitsPerSample
	 */
	std::uint8_t bitsPerSample;

	/**
	 * @since 0.0.0
	 *
	 * @property size this is the total size of the ogg file in bytes. Populated
	 * when the file is first loaded.
	 */
	ALsizei size;

	/**
	 * @since 0.0.0
	 *
	 * @property sizeConsumed is used by the callbacks to understand where in the
	 * audio data that playback is up to.
	 */
	ALsizei sizeConsumed = 0;

	/**
	 * @since 0.0.0
	 *
	 * @property format is the OpenAL enum of either MONO or STEREO
	 */
	ALenum format;

	/**
	 * @since 0.0.0
	 *
	 * @property oggVoribsFile is a handle needed by Ogg Vorbis library.
	 */
	OggVorbis_File oggVorbisFile;

	/**
	 * @since 0.0.0
	 *
	 * @property oggCurrentSection is the part of the audio that’s currently
	 * being played back.
	 */
	int oggCurrentSection = 0;

	/**
	 * @since 0.0.0
	 *
	 * @property duration is the total length of the audio data.
	 */
	std::size_t duration;

	/**
	 * @since 0.0.0
	 *
	 * @property loop indicates if the stream should loop from the begining when
	 * done.
	 */
	bool loop = false;

	/**
	 * Is the stream muted?
	 *
	 * @since 0.0.0
	 */
	bool muted = false;

	/**
	 * The stream's volume, between 0 and 1;
	 *
	 * @since 0.0.0
	 */
	ALfloat volume = 1.f;

	/**
	 * Is the stream playing?
	 * Use this instead of querying OpenAL for the source state!
	 *
	 * In case of lag, the stream will run out of buffers which will mark it as
	 * stopped in OpenAL, even though we do not want it stopped.
	 * Unlike the state, this flag only changes when the user explicitly say so.
	 * So even if the source runs out of buffers during a lag, when the program
	 * resumes, it loads up new bufffers from where it left off in the audio file.
	 *
	 * @since 0.0.0
	 */
	bool playing = false;

	/**
	 * A flag to indicate that this stream was just set up and already has buffers
	 * with data about the start of the audio file.
	 *
	 * This is used to block ONCE the rewind function that always gets called
	 * when playing a stream.
	 *
	 * @since 0.0.0
	 */
	bool justSetup = true;
};

}	// namespace Zen

#endif
