/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXT_CONST_HPP
#define ZEN_TEXT_CONST_HPP

namespace Zen {

enum class TEXT_DECORATION
{
    /**
     * @since 0.0.0
     */
    NORMAL = 0,

    /**
     * @since 0.0.0
     */
    BOLD,

    /**
     * @since 0.0.0
     */
    ITALIC,

    /**
     * @since 0.0.0
     */
    UNDERLINE,

    /**
     * @since 0.0.0
     */
    STRIKETHROUGH
};

enum class TEXT_ALIGNMENT
{
	/**
	 * @since 0.0.0
	 */
	LEFT = 0,

	/**
	 * @since 0.0.0
	 */
	RIGHT,

	/**
	 * @since 0.0.0
	 */
	CENTER
};

}	// namespace Zen

#endif
