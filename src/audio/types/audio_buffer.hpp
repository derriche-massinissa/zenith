/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_AUDIO_TYPES_AUDIOBUFFER_HPP
#define ZEN_AUDIO_TYPES_AUDIOBUFFER_HPP

#include <string>
#include "AL/al.h"

namespace Zen {

struct AudioBuffer
{
	ALuint buffer;
	std::string filename;
	std::size_t duration;
};

}	// namespace Zen

#endif
