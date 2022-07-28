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
	frameColors[frameCursor * 3]	= 0.f;
	frameColors[frameCursor * 3+1]	= 1.f;
	frameColors[frameCursor * 3+2]	= 0.f;

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

		frameColors[f * 3]		-= 0.05f * frameColors[f];
		frameColors[f * 3+1]	-= 0.05f * frameColors[f+1];
		frameColors[f * 3+2]	-= 0.05f * frameColors[f+2];

		fpsGraphImmRenderable.addQuad(quad_w, quad_h, quad_x, quad_y, frameColors[f], frameColors[f + 1], frameColors[f + 2]);	// TODO_GUI: Figure out color fade again...
	}
}