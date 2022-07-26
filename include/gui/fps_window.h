#pragma once
#include "stdafx.h"
#include "render/buffer.h"
#include "render/texture.h"
#include "render/renderable.h"

#define FRAMES_TO_TRACK 120
#define MIN_TOP_FRAME_TIME 0.0333333 // 30FPS
#define FPS_WINDOW_WIDTH 800
#define FPS_WINDOW_HEIGHT 200

struct FPSGraph
{
	double framesTracked[FRAMES_TO_TRACK];
	float quadVertices[FRAMES_TO_TRACK * 4 * 3];	// Each frame quad has 4 vertices (x, y, z)
	uint32_t quadIndices[FRAMES_TO_TRACK * 6];		// Each frame quad has 2 tris (6 indices)
	float frameColors[FRAMES_TO_TRACK * 3];
	int frameCursor = 0;

	ResourceHandle<Buffer> fpsGraphVertexBuffer;
	ResourceHandle<Buffer> fpsGraphIndexBuffer;
	ResourceHandle<Texture> fpsGraphTexture;

	Material fpsGraphMaterial;
	Renderable fpsGraphRenderable;

	void init();
	void setFrameData(double* frameData, int frameCursor);
	void update();
};