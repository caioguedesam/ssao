#include "stdafx.h"
#include "gui/fps_window.h"

//uint32_t quadIndices[] =
//{
//	0, 2, 1,
//	0, 3, 2,
//};

void FPSTimer::init()
{
	memset(framesTracked, 0, sizeof(framesTracked));
	memset(quadVertices, 0, sizeof(quadVertices));
	memset(quadIndices, 0, sizeof(quadIndices));

	// Build index array
	for (uint32_t i = 0; i < FRAMES_TO_TRACK; i += 6)
	{
		// Upper tri
		quadIndices[i]			= i;
		quadIndices[i + 1]		= i + 2;
		quadIndices[i + 2]		= i + 1;

		// Lower tri
		quadIndices[i + 3]		= i;
		quadIndices[i + 4]		= i + 3;
		quadIndices[i + 5]		= i + 2;
	}

	// Offset vertex x coordinates
	for (uint32_t i = 0; i < FRAMES_TO_TRACK; i += 12)
	{
		quadVertices[i]			= (i % 4) / FRAMES_TO_TRACK;
		quadVertices[i + 3]		= (i % 4) / FRAMES_TO_TRACK;
		quadVertices[i + 6]		= (i % 4 + 1) / FRAMES_TO_TRACK;
		quadVertices[i + 9]		= (i % 4 + 1) / FRAMES_TO_TRACK;
	}

	fpsGraphVertexBuffer = g_bufferResourceManager.createBuffer({
			BufferType::VERTEX_BUFFER,
			BufferFormat::R32_FLOAT,
			FRAMES_TO_TRACK * 4 * 3,
		}, quadVertices);
	fpsGraphIndexBuffer = g_bufferResourceManager.createBuffer({
			BufferType::INDEX_BUFFER,
			BufferFormat::R32_UINT,
			FRAMES_TO_TRACK * 6,
		}, quadIndices);

	// Setup renderable here (TODO_GUI: make renderable not necessarily follow vertex/normal/uvs)
}

void FPSTimer::update()
{
	// Get max frame time
	double top = MIN_TOP_FRAME_TIME;
	for (uint32_t i = 0; i < FRAMES_TO_TRACK; i++)
	{
		if (framesTracked[i] > top) top = framesTracked[i];
	}

	// Change vertex data
	for (uint32_t i = 0; i < FRAMES_TO_TRACK; i += 4)
	{
		double frametime = framesTracked[i];
		float relativeFrametime = static_cast<float>(frametime / top);
		// Offset vertex y coordinates
		for (uint32_t i = 0; i < FRAMES_TO_TRACK; i += 12)
		{
			quadVertices[i + 4] = relativeFrametime;
			quadVertices[i + 7] = relativeFrametime;
		}
	}
}

void FPSTimer::draw()
{
	// Rebind vertex data
	g_bufferResourceManager.bindBuffer(fpsGraphVertexBuffer);
	g_bufferResourceManager.bindBuffer(fpsGraphIndexBuffer);

	// Draw renderable
}