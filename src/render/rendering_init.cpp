#include "stdafx.h"
#include "render/rendering_resources.h"
#include "render/renderer.h"
#include "resource/material_resource_manager.h"
#include <glad/glad.h>
#include "core/math.h"
#include "random/random.h"
#include "debugging/gl.h"

namespace Ty
{
	namespace Graphics
	{
		void RenderPass_GBuffer::init(RenderTarget* rt)
		{
			this->rt = rt;

			rt->set_output(gbuffer.diffuse, 0);
			rt->set_output(gbuffer.position, 1);
			rt->set_output(gbuffer.normal, 2);
		}

		void RenderPass_SSAO::init(RenderTarget* rt)
		{
			this->rt = rt;

			generate_sample_kernel();
			generate_white_noise();

			ssao_noise_texture = texture_resource_manager.create_texture({ 4, 4, TextureFormat::R32_G32_B32_FLOAT }, &ssao_data.white_noise[0]);
			ssao_output_texture = texture_resource_manager.create_texture({ GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT, TextureFormat::R8_FLOAT }, nullptr);

			ResourceHandle<Shader> ssao_vs = shader_resource_manager.get_from_file(SHADERS_PATH"screen_quad_vs.vert");
			ResourceHandle<Shader> ssao_ps = shader_resource_manager.get_from_file(SHADERS_PATH"ssao_ps.frag");
			ShaderPipeline ssao_shader_pipeline = shader_resource_manager.create_linked_shader_pipeline(ssao_vs, ssao_ps);
			//ssao_material.init(ssao_shaderPipeline);
			ssao_material = material_resource_manager.create_material("SSAO Material", ssao_shader_pipeline);

			bind_sample_kernel();
			bind_white_noise_texture();
			bind_sample_radius();
			/*ssao_material.add_texture_to_slot(gbuffer.position, 0);
			ssao_material.add_texture_to_slot(gbuffer.normal, 1);
			ssao_material.add_texture_to_slot(ssao_noise_texture, 2);*/
			material_resource_manager.bind_texture_to_slot(ssao_material, gbuffer.position, 0);
			material_resource_manager.bind_texture_to_slot(ssao_material, gbuffer.normal, 1);
			material_resource_manager.bind_texture_to_slot(ssao_material, ssao_noise_texture, 2);

			rt->set_output(ssao_output_texture, 0);
		}

		void RenderPass_Blur::set_input_texture(ResourceHandle<Texture> input_texture)
		{
			if (blur_input_texture != input_texture)
			{
				blur_input_texture = input_texture;
				//blur_material.add_texture_to_slot(input_texture, 0);
				material_resource_manager.bind_texture_to_slot(blur_material, input_texture, 0);
			}
		}

		void RenderPass_Blur::init(RenderTarget* rt)
		{
			this->rt = rt;

			blur_output_texture = texture_resource_manager.create_texture({ GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT, TextureFormat::R8_FLOAT }, nullptr);

			ResourceHandle<Shader> blur_vs = shader_resource_manager.get_from_file(SHADERS_PATH"screen_quad_vs.vert");
			ResourceHandle<Shader> blur_ps = shader_resource_manager.get_from_file(SHADERS_PATH"ssao_blur_ps.frag");
			ShaderPipeline blur_shader_pipeline = shader_resource_manager.create_linked_shader_pipeline(blur_vs, blur_ps);
			//blur_material.init(blur_shaderPipeline);
			blur_material = material_resource_manager.create_material("Blur Material", blur_shader_pipeline);

			rt->set_output(blur_output_texture, 0);
		}

		void RenderPass_UI::init(RenderTarget* rt)
		{
			this->rt = rt;

			ui_fps_graph.init();
			rt->set_output(ui_fps_graph.fps_graph_texture, 0);
		}

		void RenderPass_Lighting::init(RenderTarget* rt)
		{
			this->rt = rt;

			lighting_output_texture = texture_resource_manager.create_texture({ GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT, TextureFormat::R8_G8_B8_A8_UNORM }, nullptr);
			ResourceHandle<Shader> lighting_vs = shader_resource_manager.get_from_file(SHADERS_PATH"screen_quad_vs.vert");
			ResourceHandle<Shader> lighting_ps = shader_resource_manager.get_from_file(SHADERS_PATH"final_pass_ps.frag");
			ShaderPipeline lighting_shader_pipeline = shader_resource_manager.create_linked_shader_pipeline(lighting_vs, lighting_ps);
			lighting_material = material_resource_manager.create_material("Lighting Material", lighting_shader_pipeline);
			material_resource_manager.bind_texture_to_slot(lighting_material, gbuffer.diffuse, 0);
			material_resource_manager.bind_texture_to_slot(lighting_material, gbuffer.position, 1);
			material_resource_manager.bind_texture_to_slot(lighting_material, gbuffer.normal, 2);

			light_count = 0;

			rt->set_output(lighting_output_texture, 0);
		}

		void RenderPass_Lighting::set_input_texture(ResourceHandle<Texture> input_texture)
		{
			if (lighting_ssao_texture != input_texture)
			{
				lighting_ssao_texture = input_texture;
				material_resource_manager.bind_texture_to_slot(lighting_material, input_texture, 3);
			}
		}

		void RenderPass_Lighting::update_directional_light(Light new_light)
		{
			/*new_light.direction = glm::normalize(new_light.direction);
			light_directional = new_light;*/
		}

		void Renderer::init_window(uint32_t w, uint32_t h, uint32_t x, uint32_t y, const char* title)
		{
			window = new Window();
			window->init(w, h, x, y, title);
		}

		void Renderer::init_api_context()
		{
			ASSERT(!gl_context_handle, "Trying to create new OpenGL context when renderer already has one.");
			ASSERT(window, "Trying to create new OpenGL context without window associated to renderer.");
			gl_context_handle = SDL_GL_CreateContext(window->handle);
			ASSERT(gl_context_handle, "Failed to create new OpenGL context.");
			SDL_GL_MakeCurrent(window->handle, gl_context_handle);
			bool result = gladLoadGLLoader(SDL_GL_GetProcAddress);
			ASSERT(result, "Failed to retrieve OpenGL API function locations using GLAD.");
		}

		void Renderer::set_viewport(Math::Primitives::u32_rect viewport_rect)
		{
			render_viewport = viewport_rect;
			GL(glViewport(render_viewport.x, render_viewport.y, render_viewport.w, render_viewport.h));
		}

		void Renderer::set_camera(float x, float y, float z, float fov, float aspect)
		{
			camera.init(x, y, z, fov, aspect);
		}

		void Renderer::init(uint32_t w, uint32_t h, uint32_t x, uint32_t y)
		{
			// Initializing API
			{
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

				SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
				SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

				SDL_GL_SetSwapInterval(0);	// No vsync
			}

			// Initializing window and context
			init_window(w, h, x, y, APP_TITLE);
			init_api_context();

			// Initializing render resource managers
			buffer_resource_manager.init();
			texture_resource_manager.init();
			shader_resource_manager.init();

			// Initializing camera
			float camera_aspect = (float)w / (float)h;
			set_camera(DEFAULT_CAMERA_X, DEFAULT_CAMERA_Y, DEFAULT_CAMERA_Z, DEFAULT_CAMERA_FOV, camera_aspect);

			// Initializing other state
			GL(glEnable(GL_DEPTH_TEST));
			GL(glEnable(GL_CULL_FACE));
			GL(glFrontFace(GL_CCW));

			set_viewport({ w, h, 0, 0 });

			// Global rendering resources
			init_global_render_resources();

			// Render passes
			pass_gbuffer.init(&rt_gbuffer);
			pass_ssao.init(&rt_ssao);
			pass_blur.init(&rt_blur);
			pass_ui.init(&rt_ui);
			pass_lighting.init(&rt_lighting);
		}

		void Renderer::destroy()
		{
			if (gl_context_handle)
			{
				SDL_GL_DeleteContext(gl_context_handle);
			}
			if (window) delete window;
		}


	}
}