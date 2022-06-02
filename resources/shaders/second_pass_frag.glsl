#version 330 core
in vec2 vTexCoord;

out vec4 outColor;

uniform sampler2D tex0;	// Diffuse
uniform sampler2D tex1;	// Position
uniform sampler2D tex2;	// Normal
uniform sampler2D tex3;	// Noise

uniform vec3 samples[64];	// Kernel

void main()
{
	outColor = texture(tex2, vTexCoord);
}