#version 330 core
in vec3 vPos;
in vec3 vNorm;
in vec2 vTexCoord;

// G-Buffer output textures
layout (location = 0) out vec4 outDiffuse;		// Diffuse
layout (location = 1) out vec3 outPos;			// World position
layout (location = 2) out vec3 outNorm;			// Normals

uniform sampler2D tex0;	// Diffuse color map
uniform sampler2D tex1;	// Normal map (TODO_#NORMAL_MAP: Support normal mapping with tangent space transformation)

void main()
{
	outDiffuse = texture(tex0, vTexCoord);
	outPos = vPos;
	outNorm = vNorm;
}