/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_COMPONENTS_WORLDVIEW_HPP
#define ZEN_COMPONENTS_WORLDVIEW_HPP

#include "../geom/types/rectangle.hpp"

namespace Zen {
namespace Components {

/**
 * The World View is a Rectangle that defines the area of the 'world' the
 * Camera is currently looking at.
 * This factors in the Camera viewport size, zoom and scroll position and
 * is updated in the Camera preRender step.
 * If you have enabled Camera bounds the worldview will be clamped to
 * those bounds accordingly.
 * You can use it for culling or intersection checks.
 *
 * @since 0.0.0
 */
struct WorldView
{
	Rectangle worldView;
};

}	// namespace Components
}	// namespace Zen

#endif
