/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_COMPONENTS_MASK_HPP
#define ZEN_COMPONENTS_MASK_HPP

#include "../ecs/entity.hpp"

namespace Zen {
namespace Components {

struct Mask
{
	Entity mask;

	/**
	 * For camera masks to enable fixed mask position that isn' influenced by
	 * the camera's transformations.
	 *
	 * @since 0.0.0
	 */
	bool fixed;
};

}	// namespace Components
}	// namespace Zen

#endif
