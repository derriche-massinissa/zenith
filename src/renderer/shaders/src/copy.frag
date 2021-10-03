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
