#version 330 core
in vec2 vTexCoord;

out vec4 outColor;		// Final output color

uniform sampler2D tex0;	// Diffuse color map
uniform sampler2D tex1;	// SSAO result texture

void main()
{
	// vec4 diffuseColor = texture(tex0, vTexCoord);
	// diffuseColor = vec4(1,1,1,1);
	// float ssaoColor = texture(tex1, vTexCoord).r;
	// outColor = diffuseColor - vec4(ssaoColor, ssaoColor, ssaoColor, 0.f);
	// outColor = texture(tex0, vTexCoord);
	float color = texture(tex1, vTexCoord).r;
	outColor = vec4(color, color, color, 1);
}