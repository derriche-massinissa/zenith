/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource->org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_SHADERS_SINGLE_VERT_HPP
#define ZEN_RENDERER_SHADERS_SINGLE_VERT_HPP

#include <string>

namespace Zen {
namespace Shaders {

const std::string SINGLE_VERT =R"(
#version 330 core

// Attributes -----------------------------------------------------------------
layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aTexId;
layout (location = 3) in float aTintEffect;
layout (location = 4) in vec4 aTint;

// Outputs --------------------------------------------------------------------
out vec2 TexCoord;
out float TintEffect;
out vec4 Tint;

// Uniforms -------------------------------------------------------------------
uniform mat4 uProjectionMatrix;

// ----------------------------------------------------------------------------
void main ()
{
	gl_Position = uProjectionMatrix * vec4(aPosition, 1.0, 1.0);

	TexCoord = aTexCoord;
	TintEffect = aTintEffect;
	Tint = aTint;
}
)";

}	// namespace Shaders
}	// namespace Zen

#endif
