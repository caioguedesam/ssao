#version 330 core
in vec2 vTexCoord;

layout (location = 0) out float ssaoResult;	// SSAO output texture (R16_UNORM)

uniform sampler2D tex0;	// Position
uniform sampler2D tex1;	// Normal
uniform sampler2D tex2;	// Noise

uniform vec3 samples[64];	// Kernel

// TODO_#SSAO_NOISE: This is hardcoded, and will break with resizing. Set this as constant uniform later.
const vec2 noiseTexScale = vec2(640.f/4.f, 480.f/4.f);

void main()
{
	ssaoResult = texture(tex0, vTexCoord).r + texture(tex2, vTexCoord * noiseTexScale).r;
}