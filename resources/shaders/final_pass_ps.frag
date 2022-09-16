#version 330 core
in vec2 vTexCoord;

out vec4 outColor;		// Final output color

uniform sampler2D tex0;	// Diffuse color map
uniform sampler2D tex1;	// SSAO result texture

uniform bool use_ssao;

void main()
{
	vec4 diff = texture(tex0, vTexCoord);
	float ssao = use_ssao ? texture(tex1, vTexCoord).r : 1;
	outColor = vec4(diff.r * ssao, diff.g * ssao, diff.b * ssao, 1);
	//outColor = vec4(color, color, color, 1);
}