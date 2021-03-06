#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;

out vec2 vTexCoord;		// Vertex UVs

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);	// No screen space transformation for default screen quad.
    vTexCoord = aUV;
}