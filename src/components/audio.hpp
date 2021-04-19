/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_COMPONENTS_AUDIO_HPP
#define ZEN_COMPONENTS_AUDIO_HPP

#include "AL/al.h"

namespace Zen {
namespace Components {

struct Audio
{
	ALuint source;
	ALuint buffer = AL_NONE;
};

}	// namespace Components
}	// namespace Zen

#endif
