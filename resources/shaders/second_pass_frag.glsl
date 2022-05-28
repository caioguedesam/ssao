#version 330 core
in vec2 vTexCoord;

out vec4 outColor;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
	outColor = texture(tex2, vTexCoord);
}