/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource->org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_SHADERS_QUAD_VERT_HPP
#define ZEN_RENDERER_SHADERS_QUAD_VERT_HPP

#include <string>

namespace Zen {
namespace Shaders {

const std::string QUAD_VERT =R"(
#version 330 core

// Attributes -----------------------------------------------------------------
layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexCoord;

// Outputs --------------------------------------------------------------------
out vec2 FragCoord;
out vec2 TexCoord;

// ----------------------------------------------------------------------------
void main ()
{
	FragCoord = aPosition.xy * 0.5 + 0.5;
	TexCoord = aTexCoord;

	gl_Position = vec4(aPosition, 0.0, 1.0);
}
)";

}	// namespace Shaders
}	// namespace Zen

#endif
