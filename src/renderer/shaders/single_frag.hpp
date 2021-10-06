/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource->org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_SHADERS_SINGLE_FRAG_HPP
#define ZEN_RENDERER_SHADERS_SINGLE_FRAG_HPP

#include <string>

namespace Zen {
namespace Shaders {

const std::string SINGLE_FRAG =R"(
#version 330 core

// Inputs ---------------------------------------------------------------------
in vec2 TexCoord;
in float TintEffect;
in vec4 Tint;

// Outputs --------------------------------------------------------------------
out vec4 FragColor;

// Uniforms -------------------------------------------------------------------
uniform sampler2D uMainSampler;

// ----------------------------------------------------------------------------
void main ()
{
	vec4 texture = texture2D(uMainSampler, TexCoord);
	vec4 texel = vec4(Tint.bgr * Tint.a, Tint.a);

	// Multiply texture tint
	vec4 color = texture * texel;

	if (TintEffect == 1.0) {
		// Solid color + texture alpha
		color.rgb = mix(texture.rgb, Tint.bgr * Tint.a, texture.a);
	}
	else if (TintEffect == 2.0) {
		// Solid color, no texture
		color = texel;
	}

	FragColor = color;
}
)";

}	// namespace Shaders
}	// namespace Zen

#endif
