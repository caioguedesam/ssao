#version 330 core
in vec2 vTexCoord;

out vec4 outColor;		// Final output color

//uniform sampler2D tex0;	// Diffuse color map
uniform sampler2D tex1;	// SSAO result texture

void main()
{
	float color = texture(tex1, vTexCoord).r;
	outColor = vec4(color, color, color, 1);
}