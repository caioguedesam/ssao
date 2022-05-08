#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 vColor;

uniform mat4 uModel;
uniform mat4 uVP;
uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vColor = vec4(aPos.xyz, 1.0);
}