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
	 * @property buffer The OpenAL buffer this source should use as an audio source.
	 */
	ALuint buffer = AL_NONE;

	/**
	 * @since 0.0.0
	 *
	 * @property completed Indicates that this source has completed.
	 * Used to emit an on complete event.
	 */
	bool completed = false;

	/**
	 * @since 0.0.0
	 *
	 * @property stopped Indicates that the audio was explicitly stopped by the
	 * user and hasn't just completed.
	 */
	bool stopped = false;

	/**
	 * @since 0.0.0
	 *
	 * @property loop Indicates that this source is looping.
	 */
	bool loop = false;

	bool muted = false;
	ALfloat volume = 1.f;
};

}	// namespace Components
}	// namespace Zen

#endif
