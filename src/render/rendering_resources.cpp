#include "stdafx.h"
#include "render/rendering_resources.h"
#include "globals.h"

namespace Ty
{
	namespace Graphics
	{
		float default_quad_vertex_data[] =
		{
			// position			// normal			// uvs
			-1.f, -1.f, 0.f,	0.f, 0.f, 0.f,		0.f, 0.f,
			-1.f, 1.f, 0.f,		0.f, 0.f, 0.f,		0.f, 1.f,
			1.f, 1.f, 0.f,		0.f, 0.f, 0.f,		1.f, 1.f,
			1.f, -1.f, 0.f,		0.f, 0.f, 0.f,		1.f, 0.f,
		};

		uint32_t default_quad_index_data[] =
		{
			0, 2, 1,
			0, 3, 2,
		};

		GBuffer gbuffer;

		ResourceHandle<Buffer> default_quad_vertex_buffer;
		ResourceHandle<Buffer> default_quad_index_buffer;

		Renderable renderable_screen_quad;

		RenderTarget rt_gbuffer;
		RenderTarget rt_ssao;
		RenderTarget rt_blur;
		RenderTarget rt_ui;
		RenderTarget rt_lighting;

		void init_global_render_resources()
		{
			// Default primitives
			default_quad_vertex_buffer = buffer_resource_manager.create_buffer(
				{
					BufferType::VERTEX_BUFFER,
					BufferFormat::R32_FLOAT,
					sizeof(default_quad_vertex_data) / sizeof(float)
				}, default_quad_vertex_data
			);
			default_quad_index_buffer = buffer_resource_manager.create_buffer(
				{
					BufferType::INDEX_BUFFER,
					BufferFormat::R32_UINT,
					sizeof(default_quad_index_data) / sizeof(uint32_t)
				}, default_quad_index_data
			);

			// Default renderables
			renderable_screen_quad.set_vertex_data(default_quad_vertex_buffer, default_quad_index_buffer);

			// G-Buffer
			gbuffer.diffuse = texture_resource_manager.create_texture({ GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT, TextureFormat::R8_G8_B8_A8_UNORM }, nullptr);
			gbuffer.position = texture_resource_manager.create_texture({ GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT, TextureFormat::R16_G16_B16_A16_FLOAT, TextureParams::WRAP_CLAMP_EDGE }, nullptr);
			gbuffer.normal = texture_resource_manager.create_texture({ GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT, TextureFormat::R16_G16_B16_A16_FLOAT }, nullptr);

			// Render targets
			rt_gbuffer.init(GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT);
			rt_ssao.init(GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT);
			rt_blur.init(GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT);
			rt_ui.init(DEBUG_FPS_WIDTH, DEBUG_FPS_HEIGHT);
			rt_lighting.init(GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT);
		}
	}
}