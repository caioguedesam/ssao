#pragma once
#include "stdafx.h"
#include "resource/buffer_resource_manager.h"

#define FRAMES_TO_TRACK 120
#define MIN_TOP_FRAME_TIME 33.3333 // 30FPS
#define FPS_WINDOW_WIDTH 800
#define FPS_WINDOW_HEIGHT 200

struct FPSTimer
{
	double framesTracked[FRAMES_TO_TRACK];
	float quadVertices[FRAMES_TO_TRACK * 4 * 3];	// Each frame quad has 4 vertices (x, y, z)
	uint32_t quadIndices[FRAMES_TO_TRACK * 6];		// Each frame quad has 2 tris (6 indices)

	ResourceHandle<Buffer> fpsGraphVertexBuffer;
	ResourceHandle<Buffer> fpsGraphIndexBuffer;

	void init();
	void update();
	void draw();

	float getHeightForFrame(const double& frameTime);
	float getFrameWidth();

	// TODO_GUI: My idea: change fps vertex/index data every frame, big buffer
	// with all bars, with normalized height from 0 to 1 and bar width. Index buffer will have sequential triangle indices.
	// After that, update vertex data for graph renderable.
};