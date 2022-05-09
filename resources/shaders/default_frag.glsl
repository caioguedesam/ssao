#version 330 core
in vec4 vColor;

out vec4 outColor;

void main()
{
	outColor = 1 - vColor;
	//outColor = vec4(1,0,0,1);
}