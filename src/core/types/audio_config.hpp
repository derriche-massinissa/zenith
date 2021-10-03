/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_CORE_CONFIG_AUDIO_HPP
#define ZEN_CORE_CONFIG_AUDIO_HPP

namespace Zen {

/*
 * Config object containing various sound settings.
 *
 * @since 0.0.0
 */
struct AudioConfig {
	/**
	 * Disable all audio output.
	 *
	 * @since 0.0.0
	 */
	bool noAudio = false;
};

}	// namespace Zen

#endif
