#include "stdafx.h"
#include "globals.h"
#include "gui/fps_window.h"
#include "resource/buffer_resource_manager.h"
#include "resource/shader_resource_manager.h"
#include "time/time.h"

namespace Ty
{
	namespace Profile
	{
		using namespace Graphics;

		void FPSGraph::init()
		{
			fpsGraphTexture = g_textureResourceManager.createTexture(
				{
					FPS_WINDOW_WIDTH,
					FPS_WINDOW_HEIGHT,
					TextureFormat::R8_G8_B8_A8_UNORM,
					TextureParams::TEXPARAMS_NONE
				}, nullptr);

			fpsGraphImmRenderable.init(FPS_WINDOW_WIDTH, FPS_WINDOW_HEIGHT);
		}

		void FPSGraph::update()		// TODO_RENDER: Move calls to this from render logic to update logic
		{
			// Get max frame time
			double top = MIN_TOP_FRAME_TIME;
			double* cpu_times = Time::FrameTracking::get_cpu_times();
			double* gpu_times = Time::FrameTracking::get_gpu_times();
			for (int i = 0; i < FRAMES_TO_TRACK; i++)
			{
				double frame_time = cpu_times[i] + gpu_times[i];
				top = max(top, frame_time);
			}

			// Add a new quad for each frame
			fpsGraphImmRenderable.clear();
			float quad_w = FPS_WINDOW_WIDTH / (float)FRAMES_TO_TRACK;
			for (uint32_t f = 0; f < FRAMES_TO_TRACK; f++)
			{
				double cpu_time = cpu_times[f];
				double gpu_time = gpu_times[f];
				float cpu_relative_time = static_cast<float>(cpu_time / top);
				float gpu_relative_time = static_cast<float>(gpu_time / top);
				float cpu_quad_h = cpu_relative_time * FPS_WINDOW_HEIGHT;
				float gpu_quad_h = gpu_relative_time * FPS_WINDOW_HEIGHT;
				float quad_x = quad_w * f;
				float cpu_quad_y = 0.f;
				float gpu_quad_y = cpu_quad_h;

				fpsGraphImmRenderable.addQuad(quad_w, cpu_quad_h, quad_x, cpu_quad_y, 0, 0, 1, 1);
				fpsGraphImmRenderable.addQuad(quad_w, gpu_quad_h, quad_x, gpu_quad_y, 0, 1, 0, 1);
			}

			// Add standard frame time marks (60fps, 120fps)
			float relativeFrametime_60 = static_cast<float>(16.666 / top);
			float relativeFrametime_120 = static_cast<float>(8.333 / top);
			fpsGraphImmRenderable.addQuad(FPS_WINDOW_WIDTH, 1.f, 0.f, relativeFrametime_60 * FPS_WINDOW_HEIGHT, 1.f, 1.f, 1.f, 0.5f);
			fpsGraphImmRenderable.addQuad(FPS_WINDOW_WIDTH, 1.f, 0.f, relativeFrametime_120 * FPS_WINDOW_HEIGHT, 1.f, 1.f, 1.f, 0.5f);
		}
	}
}