/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource->org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_SHADERS_MESH_FRAG_HPP
#define ZEN_RENDERER_SHADERS_MESH_FRAG_HPP

#include <string>

namespace Zen {
namespace Shaders {

const std::string MESH_FRAG =R"(
#version 330 core

// Inputs ---------------------------------------------------------------------
in vec2 TexCoord;
in vec3 Normal;
in vec3 Position;

// Outputs --------------------------------------------------------------------
out vec4 FragColor;

// Uniforms -------------------------------------------------------------------
uniform vec3 uLightPosition;
uniform vec3 uLightAmbient;
uniform vec3 uLightDiffuse;
uniform vec3 uLightSpecular;

uniform vec3 uFogColor;
uniform float uFogNear;
uniform float uFogFar;

uniform vec3 uMaterialAmbient;
uniform vec3 uMaterialDiffuse;
uniform vec3 uMaterialSpecular;
uniform float uMaterialShine;

uniform vec3 uCameraPosition;

uniform sampler2D uTexture;

// ----------------------------------------------------------------------------
void main ()
{
	vec4 color = texture2D(uTexture, TexCoord);

	vec3 ambient = uLightAmbient * uMaterialAmbient;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(uLightPosition - Position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = uLightDiffuse * (diff * uMaterialDiffuse);

	vec3 viewDir = normalize(uCameraPosition - Position);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterialShine);
	vec3 specular = uLightSpecular * (spec * uMaterialSpecular);

	vec3 result = (ambient + diffuse + specular) * color.rgb;

	float depth = gl_FragCoord.z / gl_FragCoord.w;

	float fogFactor = smoothstep(uFogNear, uFogFar, depth);

	FragColor.rgb = mix(result.rgb, uFogColor, fogFactor);
	FragColor.a = color.a;
}
)";

}	// namespace Shaders
}	// namespace Zen

#endif
