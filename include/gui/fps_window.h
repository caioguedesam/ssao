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
	float frameColors[FRAMES_TO_TRACK * 3];	// rgb
	int frameCursor = 0;

	ResourceHandle<Texture> fpsGraphTexture;

	ImmediateRenderable fpsGraphImmRenderable;

	void init();
	void setFrameData(double* frameData, int frameCursor);
	void update();
};