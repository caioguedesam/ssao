#version 330 core
in vec2 vTexCoord;

layout (location = 0) out float ssaoResult;	// SSAO output texture (R16_UNORM)

uniform sampler2D tex0;	// Position
uniform sampler2D tex1;	// Normal
uniform sampler2D tex2;	// Noise

uniform vec3 samples[64];	// Kernel

uniform mat4 uProj;

// TODO_#SSAO_NOISE: This is hardcoded, and will break with resizing. Set this as constant uniform later.
const vec2 noiseTexScale = vec2(1280.0/4.0, 720.0/4.0);
float bias = 0.025;
int kernelSize = 64;	// Kernel size is hardcoded
float radius = 0.5;

void main()
{
	vec3 fragmentPosition = texture(tex0, vTexCoord).xyz;
	vec3 fragmentNormal = normalize(texture(tex1, vTexCoord).rgb);
	vec3 tilt = normalize(texture(tex2, vTexCoord * noiseTexScale).xyz);

	// Tangent to View space matrix
	vec3 tangent = normalize(tilt - fragmentNormal * dot(tilt, fragmentNormal));
	vec3 bitangent = cross(fragmentNormal, tangent);
	mat3 TBN = mat3(tangent, bitangent, fragmentNormal);

	float occlusion = 0.0;
	for(int i = 0; i < kernelSize; i++)
	{	
		// Get sample position in view space
		vec3 samplePosition = TBN * samples[i];
		samplePosition = fragmentPosition + samplePosition * radius;

		// Project sample position to screen space
		vec4 offset = vec4(samplePosition, 1.0);
		offset = uProj * offset;	// View -> clip
		offset.xyz /= offset.w;		// Perspective divide
		offset.xyz = offset.xyz * 0.5 + 0.5;		// [0 - 1]

		// Get depth at sample screen position
		float sampleDepth = texture(tex0, offset.xy).z;

		// Accumulate
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragmentPosition.z - sampleDepth));
		occlusion += (sampleDepth >= samplePosition.z + bias ? 1.0 : 0.0) * rangeCheck;
	}

	occlusion = 1.0 - (occlusion / kernelSize);

	ssaoResult = occlusion;
}