/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource->org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_SHADERS_ADD_BLEND_FRAG_HPP
#define ZEN_RENDERER_SHADERS_ADD_BLEND_FRAG_HPP

#include <string>

namespace Zen {
namespace Shaders {

const std::string ADD_BLEND_FRAG =R"(
#version 330 core

// Inputs ---------------------------------------------------------------------
in vec2 TexCoord;

// Outputs --------------------------------------------------------------------
out vec4 FragColor;

// Uniforms -------------------------------------------------------------------
uniform sampler2D uMainSampler1;
uniform sampler2D uMainSampler2;
uniform float uStrength;

// ----------------------------------------------------------------------------
void main ()
{
	vec4 frame1 = texture2D(uMainSampler1, TexCoord);
	vec4 frame2 = texture2D(uMainSampler2, TexCoord);

	FragColor = frame1 + frame2 * uStrength;
}
)";

}	// namespace Shaders
}	// namespace Zen

#endif
