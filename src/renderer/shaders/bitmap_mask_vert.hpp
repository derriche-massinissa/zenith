/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource->org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_SHADERS_BITMAP_MASK_VERT_HPP
#define ZEN_RENDERER_SHADERS_BITMAP_MASK_VERT_HPP

#include <string>

namespace Zen {
namespace Shaders {

const std::string BITMAP_MASK_VERT =R"(
#version 330 core

// Attributes -----------------------------------------------------------------
layout (location = 0) in vec2 aPosition;

// ----------------------------------------------------------------------------
void main ()
{
	gl_Position = vec4(aPosition, 0.0, 1.0);
}
)";

}	// namespace Shaders
}	// namespace Zen

#endif
