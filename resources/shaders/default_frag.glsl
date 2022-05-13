#version 330 core
in vec4 vColor;
in vec2 vTexCoord;

out vec4 outColor;

uniform sampler2D tex0;

void main()
{
	//outColor = 1 - vColor;
	//outColor = vec4(vTexCoord.x, vTexCoord.y,0,1);
	outColor = texture(tex0, vTexCoord);
}