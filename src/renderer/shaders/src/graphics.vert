#version 330 core

// Attributes -----------------------------------------------------------------
layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec4 aColor;

// Outputs --------------------------------------------------------------------
out vec4 Color;

// Uniforms -------------------------------------------------------------------
uniform mat4 uProjectionMatrix;

// ----------------------------------------------------------------------------
void main ()
{
	gl_Position = uProjectionMatrix * vec4(aPosition, 1.0, 1.0);

	Color = aColor;
}
