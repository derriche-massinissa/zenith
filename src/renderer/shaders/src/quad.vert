#version 330 core

// Attributes -----------------------------------------------------------------
layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexCoord;

// Outputs --------------------------------------------------------------------
out vec2 FragCoord;
out vec2 TexCoord;

// ----------------------------------------------------------------------------
void main ()
{
	FragCoord = aPosition.xy * 0.5 + 0.5;
	TexCoord = aTexCoord;

	gl_Position = vec4(aPosition, 0.0, 1.0);
}
