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
const float bias = 0.025;

void main()
{
	vec3 pos = texture(tex0, vTexCoord).xyz;
	vec3 normal = texture(tex1, vTexCoord).xyz;
	vec3 tilt = texture(tex2, vTexCoord * noiseTexScale).xyz;

	// Convert samples from tangent to view space
	vec3 tangent = normalize(tilt - normal * dot(tilt, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);

	float occlusion = 0.0;
	for(int i = 0; i < 64; i++)		// Kernel size is hardcoded
	{
		vec4 samplePos = vec4(TBN * samples[i], 1.0);
		// Convert samples from view to screen space
		samplePos = uProj * samplePos;	// view -> clip
		samplePos.xyz /= samplePos.w;	// perspective divide
		samplePos.xyz = samplePos.xyz * 0.5 + 0.5;	// [0 - 1]

		float sampleDepth = texture(tex0, samplePos.xy).z;
		occlusion += (sampleDepth >= pos.z + bias ? 1.0 : 0.0);
	}
	occlusion = 1.0 - (occlusion / 64.0);		// Kernel size is hardcoded

	// //ssaoResult = texture(tex0, vTexCoord).r + texture(tex2, vTexCoord * noiseTexScale).r;
	ssaoResult = occlusion;
}