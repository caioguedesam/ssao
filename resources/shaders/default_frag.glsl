#version 330 core
in vec3 vPos;
in vec3 vNorm;
in vec4 vColor;
in vec2 vTexCoord;

layout (location = 0) out vec4 outDiffuse;
layout (location = 1) out vec3 outPos;
layout (location = 2) out vec3 outNorm;

uniform sampler2D tex0;	// Diffuse map
uniform sampler2D tex1;	// Normal map

void main()
{
	outDiffuse = texture(tex0, vTexCoord);
	outPos = vPos;
	// TODO: Figure out normal mapping and tangent space
	//outNorm = texture(tex1, vTexCoord).xyz;
	//outNorm = normalize(outNorm * 2.0 - 1.0);
	outNorm = vNorm;
}