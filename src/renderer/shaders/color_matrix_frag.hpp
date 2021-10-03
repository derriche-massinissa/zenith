/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource->org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_SHADERS_COLOR_MATRIX_FRAG_HPP
#define ZEN_RENDERER_SHADERS_COLOR_MATRIX_FRAG_HPP

#include <string>

namespace Zen {
namespace Shaders {

const std::string COLOR_MATRIX_FRAG =R"(
#version 330 core

// Inputs ---------------------------------------------------------------------
in vec2 TexCoord;

// Outputs --------------------------------------------------------------------
out vec4 FragColor;

// Uniforms -------------------------------------------------------------------
uniform sampler2D uMainSampler;
uniform float uColorMatrix[20];
uniform float uAlpha;

// ----------------------------------------------------------------------------
void main ()
{
	vec4 c = texture2D(uMainSampler, TexCoord);

	if (uAlpha == 0.0) {
		FragColor = c;
		return;
	}

	if (c.a > 0.0)
		c.rgb /= c.a;

	vec4 result;

    result.r = (uColorMatrix[0] * c.r) + (uColorMatrix[1] * c.g) +
		(uColorMatrix[2] * c.b) + (uColorMatrix[3] * c.a) + uColorMatrix[4];
    result.g = (uColorMatrix[5] * c.r) + (uColorMatrix[6] * c.g) +
		(uColorMatrix[7] * c.b) + (uColorMatrix[8] * c.a) + uColorMatrix[9];
    result.b = (uColorMatrix[10] * c.r) + (uColorMatrix[11] * c.g) +
		(uColorMatrix[12] * c.b) + (uColorMatrix[13] * c.a) + uColorMatrix[14];
    result.a = (uColorMatrix[15] * c.r) + (uColorMatrix[16] * c.g) +
		(uColorMatrix[17] * c.b) + (uColorMatrix[18] * c.a) + uColorMatrix[19];

	vec3 rgb = mix(c.rgb, result.rgb, uAlpha);

	rgb *= result.a;

	FragColor = vec4(rgb, result.a);
}
)";

}	// namespace Shaders
}	// namespace Zen

#endif
