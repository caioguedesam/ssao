#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;

out vec3 vPos;			// Vertex world space position
out vec3 vNorm;			// Vertex world space normal
out vec2 vTexCoord;		// Vertex UVs

uniform mat4 uModel;
uniform mat4 uMV;
uniform mat4 uVP;
uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	
	vPos = (uModel * vec4(aPos.xyz, 1.0)).xyz;
	vNorm = normalize(aNorm);
	vTexCoord = aUV;
}