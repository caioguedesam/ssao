#version 330 core
in vec2 vTexCoord;

out vec4 outColor;

uniform sampler2D texDiffuse;
uniform sampler2D texPosition;
uniform sampler2D texNormal;

void main()
{
	outColor = texture(texNormal, vTexCoord);
}