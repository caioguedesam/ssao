#pragma once
#include "stdafx.h"
#include "resource/buffer_resource_manager.h"
#include "resource/texture_resource_manager.h"
#include "resource/shader_resource_manager.h"
#include "render/render_target.h"
#include "render/renderable.h"
#include "render/camera.h"

namespace Ty
{
	namespace Graphics
	{
		struct GBuffer
		{
			ResourceHandle<Texture> diffuse;
			ResourceHandle<Texture> position;
			ResourceHandle<Texture> normal;
		};
		extern GBuffer gbuffer;

		extern ResourceHandle<Buffer> default_quad_vertex_buffer;
		extern ResourceHandle<Buffer> default_quad_index_buffer;

		extern Renderable renderable_screen_quad;

		extern RenderTarget rt_gbuffer;
		extern RenderTarget rt_ssao;
		extern RenderTarget rt_blur;
		extern RenderTarget rt_ui;
		extern RenderTarget rt_lighting;

		void init_global_render_resources();
	}
}