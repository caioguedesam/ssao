#include "stdafx.h"
#include "globals.h"
#include "gui/fps_window.h"
#include "resource/buffer_resource_manager.h"
#include "resource/shader_resource_manager.h"

void FPSGraph::init()
{
	memset(framesTracked, 0, sizeof(framesTracked));
	memset(frameColors, 0, sizeof(frameColors));
	fpsGraphTexture = g_textureResourceManager.createTexture(
		{
			FPS_WINDOW_WIDTH,
			FPS_WINDOW_HEIGHT,
			TextureFormat::R8_G8_B8_A8_UNORM,
			TextureParams::TEXPARAMS_NONE
		}, nullptr);

	fpsGraphImmRenderable.init(FPS_WINDOW_WIDTH, FPS_WINDOW_HEIGHT);
}

void FPSGraph::setFrameData(double* frameData, int frameCursor)
{
	for (int i = 0; i < FRAMES_TO_TRACK; i++)
	{
		framesTracked[i] = frameData[i];
	}
	this->frameCursor = frameCursor;
}

void FPSGraph::update()
{
	// Get max frame time
	double top = MIN_TOP_FRAME_TIME;
	for (int i = 0; i < FRAMES_TO_TRACK; i++)
	{
		if (framesTracked[i] > top) top = framesTracked[i];
	}

	// Update color array
	frameColors[frameCursor * 4]	= 0.f;
	frameColors[frameCursor * 4+1]	= 1.f;
	frameColors[frameCursor * 4+2]	= 0.f;
	frameColors[frameCursor * 4+3]	= 1.f;

	// Add a new quad for each frame
	fpsGraphImmRenderable.clear();
	float quad_w = FPS_WINDOW_WIDTH / (float)FRAMES_TO_TRACK;
	for (uint32_t f = 0; f < FRAMES_TO_TRACK; f++)
	{
		double frametime = framesTracked[f];
		float relativeFrametime = static_cast<float>(frametime / top);
		float quad_h = relativeFrametime * FPS_WINDOW_HEIGHT;
		float quad_x = quad_w * f;
		float quad_y = 0.f;

		frameColors[f * 4]		-= 0.005f * frameColors[f * 4];
		frameColors[f * 4+1]	-= 0.005f * frameColors[f * 4+1];
		frameColors[f * 4+2]	-= 0.005f * frameColors[f * 4+2];
		frameColors[f * 4+3]	-= 0.005f * frameColors[f * 4+3];

		fpsGraphImmRenderable.addQuad(quad_w, quad_h, quad_x, quad_y, frameColors[f * 4], frameColors[f * 4 + 1], frameColors[f * 4 + 2], frameColors[f * 4 + 3]);
	}

	// Add standard frame time marks (60fps, 120fps)
	float relativeFrametime_60 = static_cast<float>(0.016666 / top);
	float relativeFrametime_120 = static_cast<float>(0.008333 / top);
	fpsGraphImmRenderable.addQuad(FPS_WINDOW_WIDTH, 1.f, 0.f, relativeFrametime_60 * FPS_WINDOW_HEIGHT, 1.f, 1.f, 1.f, 0.5f);
	fpsGraphImmRenderable.addQuad(FPS_WINDOW_WIDTH, 1.f, 0.f, relativeFrametime_120 * FPS_WINDOW_HEIGHT, 1.f, 1.f, 1.f, 0.5f);
}