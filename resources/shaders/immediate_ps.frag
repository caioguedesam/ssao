#version 330 core
in vec3 vColor;

layout (location = 0) out vec4 outColor;

void main()
{
	outColor = vec4(vColor.rgb, 1);
}