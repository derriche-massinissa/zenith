#version 330 core

// Inputs ---------------------------------------------------------------------
in vec2 TexCoord;
in float TintEffect;
in vec4 Tint;

// Outputs --------------------------------------------------------------------
out vec4 FragColor;

// Uniforms -------------------------------------------------------------------
uniform sampler2D uMainSampler;

// ----------------------------------------------------------------------------
void main ()
{
	vec4 texture = texture2D(uMainSampler, TexCoord);
	vec4 texel = vec4(Tint.bgr * Tint.a, Tint.a);

	// Multiply texture tint
	vec4 color = texture * texel;

	if (TintEffect == 1.0) {
		// Solid color + texture alpha
		color.rgb = mix(texture.rgb, Tint.bgr * Tint.a, texture.a);
	}
	else if (TintEffect == 2.0) {
		// Solid color, no texture
		color = texel;
	}

	FragColor = color;
}
