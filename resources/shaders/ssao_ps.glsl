#version 330 core
in vec2 vTexCoord;

layout (location = 0) out float ssaoResult;	// SSAO output texture (R16_UNORM)

uniform sampler2D tex0;	// Position
uniform sampler2D tex1;	// Normal
uniform sampler2D tex2;	// Noise

uniform vec3 samples[64];	// Kernel

void main()
{
	ssaoResult = texture(tex0, vTexCoord).r;
}