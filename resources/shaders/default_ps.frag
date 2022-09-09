#version 330 core
in vec3 vPos;
in vec3 vNorm;
in vec2 vTexCoord;

// G-Buffer output textures
layout (location = 0) out vec4 outDiffuse;		// Diffuse
layout (location = 1) out vec3 outPos;			// World position
layout (location = 2) out vec3 outNorm;			// Normals

uniform sampler2D tex0;	// Diffuse color map
// uniform sampler2D tex1;	// Normal map (TODO_#NORMAL_MAP: Support normal mapping with tangent space transformation)

uniform mat4 uView;

void main()
{
	vec4 diff_color = texture(tex0, vTexCoord);
	if(diff_color.a < 0.1f) discard;
	
	outDiffuse = diff_color;
	//outDiffuse = vec4(1,1,1,1);
	//outPos = (uView * vec4(vPos, 1)).xyz;		// G-Buffer textures are stored in view space (for SSAO pass).
	outPos = vPos;
	outNorm = normalize(vNorm);
}