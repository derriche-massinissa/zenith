/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource->org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_SHADERS_MESH_VERT_HPP
#define ZEN_RENDERER_SHADERS_MESH_VERT_HPP

#include <string>

namespace Zen {
namespace Shaders {

const std::string MESH_VERT =R"(
#version 330 core

// Attributes -----------------------------------------------------------------
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

// Outputs --------------------------------------------------------------------
out vec2 TexCoord;
out vec3 Normal;
out vec3 Position;

// Uniforms -------------------------------------------------------------------
uniform mat4 uViewProjectionMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uNormalMatrix;

// ----------------------------------------------------------------------------
void main ()
{
	TexCoord = aTexCoord;

	Position =  vec3(uModelMatrix * vec4(aPosition, 1.0));

	Normal = vec3(uNormalMatrix * vec4(aNormal, 1.0));

	gl_Position = uViewProjectionMatrix * uModelMatrix * vec4(aPosition, 1.0);
}
)";

}	// namespace Shaders
}	// namespace Zen

#endif
