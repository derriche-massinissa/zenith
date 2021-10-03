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
