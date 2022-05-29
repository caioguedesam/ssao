#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;

out vec3 vPos;
out vec3 vNorm;
out vec4 vColor;
out vec2 vTexCoord;

uniform mat4 uModel;
uniform mat4 uMV;
uniform mat4 uVP;
uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	//vPos = gl_Position.xyz;
	vPos = (uModel * vec4(aPos.xyz, 1.0)).xyz;	// Position in world space
	
	vNorm = aNorm;
	
	vColor = vec4(aPos.xyz, 1.0);
	vTexCoord = aUV;
}