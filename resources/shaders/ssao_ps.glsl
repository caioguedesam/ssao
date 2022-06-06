#version 330 core
in vec2 vTexCoord;

layout (location = 0) out float ssaoResult;	// SSAO output texture (R16_UNORM)

uniform sampler2D tex0;	// Position
uniform sampler2D tex1;	// Normal
uniform sampler2D tex2;	// Noise

uniform vec3 samples[64];	// Kernel

uniform mat4 uView;
uniform mat4 uProj;

// TODO_#SSAO_NOISE: This is hardcoded, and will break with resizing. Set this as constant uniform later.
const vec2 noiseTexScale = vec2(640.f/4.f, 480.f/4.f);

void main()
{
	vec3 pos = texture(tex0, vTexCoord).xyz;
	vec3 normal = texture(tex1, vTexCoord).xyz;
	vec3 tilt = texture(tex2, vTexCoord).xyz;

	// Convert samples from tangent to view space
	vec3 tangent = normalize(tilt - normal * dot(tilt, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);

	float occlusion = 0.f;
	for(int i = 0; i < 64; i++)		// Sample size is hardcoded
	{
		vec3 samplePos = TBN * samples[i];
		// Convert samples from view to screen space
	}

	ssaoResult = texture(tex0, vTexCoord).r + texture(tex2, vTexCoord * noiseTexScale).r;
}