/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource->org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_SHADERS_COPY_FRAG_HPP
#define ZEN_RENDERER_SHADERS_COPY_FRAG_HPP

#include <string>

namespace Zen {
namespace Shaders {

const std::string COPY_FRAG =R"(
#version 330 core

// Inputs ---------------------------------------------------------------------
in vec4 TexCoord;

// Outputs --------------------------------------------------------------------
out vec4 FragColor;

// Uniforms -------------------------------------------------------------------
uniform sampler2D uMainSampler;
uniform float uBrightness;

// ----------------------------------------------------------------------------
void main ()
{
	FragColor = texture2D(uMainSampler, TexCoord) * uBrightness;
}
)";

}	// namespace Shaders
}	// namespace Zen

#endif
