/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 *
 * Source:
 * https://indiegamedev.net/2020/01/16/how-to-stream-ogg-files-with-openal-in-c/
 */

#ifndef ZEN_AUDIO_TOOLS_OGG_HPP
#define ZEN_AUDIO_TOOLS_OGG_HPP

#include <bits/c++config.h>
#include <cstdint>
#include <string>
#include <array>
#include <fstream>
#include <vorbis/vorbisfile.h>
#include "al_utility.hpp"
#include "alc_utility.hpp"
#include "../types/audio_buffer.hpp"
#include "../types/audio_stream_data.hpp"

#define OGG_AL_NUM_BUFFERS 4
#define OGG_AL_BUFFER_SIZE 65536

namespace Zen {

/**
 * @since 0.0.0
 *
 * @param filename The audio file to load.
 * @param audioData A poitner to the AudioBuffer object to store the audio in.
 *
 * @return `0` if no problem occured, or a negative number otherwise.
 */
int load_ogg (const std::string& filename, AudioBuffer *audioData);

int setup_stream_ogg (const std::string& filename, AudioStreamData *audioStream);

/**
 * @return 1 if stream completed, 2 if it looped, 0 otherwise.
 */
int update_stream_ogg (AudioStreamData *audioStream);

int rewind_stream_ogg (AudioStreamData *audioStream);

void close_stream_ogg (AudioStreamData *audioStream);

}	// namespace Zen

#endif
