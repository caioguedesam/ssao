#pragma once
#include "stdafx.h"
#include "render/buffer.h"
#include "render/texture.h"
#include "render/renderable.h"

#define FRAMES_TO_TRACK 240
#define MIN_TOP_FRAME_TIME 33.3333 // 30FPS
#define FPS_WINDOW_WIDTH 800
#define FPS_WINDOW_HEIGHT 200

namespace Ty
{
	namespace Profile
	{
		struct FPSGraph
		{
			Graphics::ResourceHandle<Graphics::Texture> fps_graph_texture;
			Graphics::ImmediateRenderable fps_graph_renderable;

			void init();
			void update();
		};
	}
}