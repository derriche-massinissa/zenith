/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource->org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_SHADERS_GRAPHICS_FRAG_HPP
#define ZEN_RENDERER_SHADERS_GRAPHICS_FRAG_HPP

#include <string>

namespace Zen {
namespace Shaders {

const std::string GRAPHICS_FRAG =R"(
#version 330 core

// Inputs ---------------------------------------------------------------------
in vec4 Color;

// Outputs --------------------------------------------------------------------
out vec4 FragColor;

// ----------------------------------------------------------------------------
void main ()
{
	FragColor = vec4(Color.bgr * Color.a, Color.a);
}
)";

}	// namespace Shaders
}	// namespace Zen

#endif
