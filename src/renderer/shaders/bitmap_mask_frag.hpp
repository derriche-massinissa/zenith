/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource->org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_SHADERS_BITMAP_MASK_FRAG_HPP
#define ZEN_RENDERER_SHADERS_BITMAP_MASK_FRAG_HPP

#include <string>

namespace Zen {
namespace Shaders {

const std::string BITMAP_MASK_FRAG =R"(
#version 330 core

// Outputs --------------------------------------------------------------------
out vec4 FragColor;

// Uniforms -------------------------------------------------------------------
uniform vec2 uResolution;
uniform sampler2D uMainSampler;
uniform sampler2D uMaskSampler;
uniform bool uInvertMaskAlpha;

// ----------------------------------------------------------------------------
void main ()
{
	vec2 uv = gl_FragCoord.xy / uResolution;
	vec4 mainColor = texture2D(uMainSampler, uv);
	vec4 maskColor = texture2D(uMaskSampler, uv);
	float alpha = mainColor.a;

	if (!uInvertMaskAlpha)
		alpha *= maskColor.a;
	else
		alpha *= (1.0 - maskColor.a);

	FragColor = vec4(mainColor.rgb * alpha, alpha);
}
)";

}	// namespace Shaders
}	// namespace Zen

#endif
