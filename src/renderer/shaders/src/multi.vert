#version 330 core

// Attributes -----------------------------------------------------------------
layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aTexId;
layout (location = 3) in float aTintEffect;
layout (location = 4) in vec4 aTint;

// Outputs --------------------------------------------------------------------
out vec2 TexCoord;
out float TexId;
out float TintEffect;
out vec4 Tint;

// Uniforms -------------------------------------------------------------------
uniform mat4 uProjectionMatrix;

// ----------------------------------------------------------------------------
void main ()
{
	gl_Position = uProjectionMatrix * vec4(aPosition, 1.0, 1.0);

	TexCoord = aTexCoord;
	TexId = aTexId;
	TintEffect = aTintEffect;
	Tint = aTint;
}
