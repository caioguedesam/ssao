#version 330 core

#define FRAMES_TO_TRACK 120

in vec3 vPos;
layout (location = 0) out vec4 outDiffuse;

uniform float frameColors[FRAMES_TO_TRACK * 3];

void main()
{
	// Get index based on x vertex pos from -1 to 1
	int ix = int((vPos.x + 1.0) * 0.5);	// -1..1 to 0..1

	outDiffuse = vec4(frameColors[ix], frameColors[ix+1], frameColors[ix+2], 1.0);
}