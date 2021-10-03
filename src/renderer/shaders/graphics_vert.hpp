/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource->org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_SHADERS_GRAPHICS_VERT_HPP
#define ZEN_RENDERER_SHADERS_GRAPHICS_VERT_HPP

#include <string>

namespace Zen {
namespace Shaders {

const std::string GRAPHICS_VERT =R"(
#version 330 core

// Attributes -----------------------------------------------------------------
layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec4 aColor;

// Outputs --------------------------------------------------------------------
out vec4 Color;

// Uniforms -------------------------------------------------------------------
uniform mat4 uProjectionMatrix;

// ----------------------------------------------------------------------------
void main ()
{
	gl_Position = uProjectionMatrix * vec4(aPosition, 1.0, 1.0);

	Color = aColor;
}
)";

}	// namespace Shaders
}	// namespace Zen

#endif
