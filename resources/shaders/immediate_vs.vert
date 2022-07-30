#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec4 vColor;

uniform int viewportWidth;
uniform int viewportHeight;

void main()
{
    float pos_x = (aPos.x / viewportWidth) * 2 - 1;
    float pos_y = (aPos.y / viewportHeight) * 2 - 1;
    gl_Position = vec4(pos_x, pos_y, aPos.z, 1.0);
    vColor = aColor;
}