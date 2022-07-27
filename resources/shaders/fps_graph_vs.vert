#version 330 core
layout (location = 0) in vec3 iPos;     // Normalized position

out vec3 vPos;			// Normalized position

void main()
{
	gl_Position = vec4(iPos, 1.0);
    vPos = iPos;
}