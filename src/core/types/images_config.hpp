/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_CORE_CONFIG_IMAGES_HPP
#define ZEN_CORE_CONFIG_IMAGES_HPP

#include <string>

namespace Zen {

/*
 * Config object containing various sound settings.
 *
 * @since 0.0.0
 */
struct ImagesConfig {
	/**
	 * A base64 encoded image file to use as the 'default' texture.
	 *
	 * @since 0.0.0
	 */
	std::string defaults;

	/**
	 * A base64 encoded image file to use as the 'missing' texture.
	 *
	 * @since 0.0.0
	 */
	std::string missing;

	/**
	 * base64 encoded image file to use as the 'white' texture.
	 *
	 * @since 0.0.0
	 */
	std::string white;
};

}	// namespace Zen

#endif
