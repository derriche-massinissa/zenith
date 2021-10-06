#version 330 core

// Inputs ---------------------------------------------------------------------
in vec2 TexCoord;

// Outputs --------------------------------------------------------------------
out vec4 FragColor;

// Uniforms -------------------------------------------------------------------
uniform sampler2D uMainSampler1;
uniform sampler2D uMainSampler2;
uniform float uStrength;

// ----------------------------------------------------------------------------
void main ()
{
	vec4 frame1 = texture2D(uMainSampler1, TexCoord);
	vec4 frame2 = texture2D(uMainSampler2, TexCoord);

	FragColor = mix(frame1, frame2 * uStrength, 0.5);
}
