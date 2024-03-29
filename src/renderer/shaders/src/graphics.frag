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
