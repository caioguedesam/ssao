#include "stdafx.h"
#include "app.h"

#include <SDL.h>
#include "glm/gtx/transform.hpp"

#include "time/time.h"
#include "resource/resource_loader.h"
#include "resource/buffer_resource_manager.h"
#include "resource/texture_resource_manager.h"
#include "resource/shader_resource_manager.h"
#include "gui/gui.h"

namespace Ty
{
	// TODO_MULTIPLATFORM: Remove this SDL dependency and use platform api
	void get_display_dimensions(uint32_t& w, uint32_t& h)
	{
		ASSERT(SDL_WasInit(SDL_INIT_VIDEO), "Trying to get display dimensions without initializing SDL.");
		SDL_DisplayMode DM;
		SDL_GetDesktopDisplayMode(0, &DM);
		w = DM.w;
		h = DM.h;
	}

	void App::init()
	{
		uint32_t app_w = APP_DEFAULT_WIDTH;
		uint32_t app_h = APP_DEFAULT_HEIGHT;
		get_display_dimensions(screen_w, screen_h);

		// Core engine systems initialization
		Time::init();
		Input::InputManager::init();

		// Rendering system initialization
		renderer.init(app_w, app_h, (screen_w - app_w) / 2, (screen_h - app_h) / 2);

		// Resource system initialization
		Graphics::texture_resource_manager.init();
		Graphics::shader_resource_manager.init();

		UI::GUI::init(this);

		is_running = true;
	}

	void App::poll_events(double dt)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			using namespace Input;

			UI::GUI::process_event(&event);
			if (event.type == SDL_KEYDOWN)
			{
				SDL_Keysym key = event.key.keysym;
				switch (key.sym)
				{
				case SDLK_ESCAPE:
				{
					is_running = false;
				} break;
				case SDLK_p:
				{
					InputManager::change_input_mode(InputManager::current_state == InputManager::State::APP ? InputManager::State::GUI : InputManager::State::APP);
				} break;
				case SDLK_w:
				{
					renderer.camera.move_amounts.y = 1;
				} break;
				case SDLK_s:
				{
					renderer.camera.move_amounts.y = -1;
				} break;
				case SDLK_a:
				{
					renderer.camera.move_amounts.x = -1;
				} break;
				case SDLK_d:
				{
					renderer.camera.move_amounts.x = 1;
				} break;
				case SDLK_UP:
				{
					renderer.camera.rotate_amounts.x = 1;
				} break;
				case SDLK_DOWN:
				{
					renderer.camera.rotate_amounts.x = -1;
				} break;
				case SDLK_LEFT:
				{
					renderer.camera.rotate_amounts.y = -1;
				} break;
				case SDLK_RIGHT:
				{
					renderer.camera.rotate_amounts.y = 1;
				} break;
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				SDL_Keysym key = event.key.keysym;
				switch (key.sym)
				{
				case SDLK_w:
				case SDLK_s:
				{
					renderer.camera.move_amounts.y = 0;
				} break;
				case SDLK_a:
				case SDLK_d:
				{
					renderer.camera.move_amounts.x = 0;
				} break;
				case SDLK_UP:
				case SDLK_DOWN:
				{
					renderer.camera.rotate_amounts.x = 0;
				} break;
				case SDLK_LEFT:
				case SDLK_RIGHT:
				{
					renderer.camera.rotate_amounts.y = 0;
				} break;
				}
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				InputManager::update_mouse_data(event);
			}
			else if (event.type == SDL_WINDOWEVENT)
			{
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					uint32_t newWidth = event.window.data1;
					uint32_t newHeight = event.window.data2;
					//renderer.OnResize(newWidth, newHeight);	// TODO_RESIZE: Renderer shouldn't be resizing (default 1080p). What has to resize is the gui window w/ final texture.
				}
			}
			else if (event.type == SDL_QUIT)
			{
				is_running = false;
			}
		}
	}

	void App::run()
	{
		AssetSystem::Model sponza;
		AssetSystem::ResourceLoader::load_model(sponza, MODELS_PATH"sponza.obj");

		Graphics::ResourceHandle<Graphics::Buffer> vertex_buffer = Graphics::buffer_resource_manager.create_buffer({ Graphics::BufferType::VERTEX_BUFFER, Graphics::BufferFormat::R32_FLOAT, sponza.vertices.size() }, sponza.vertices.data());
		Graphics::ResourceHandle<Graphics::Buffer> index_buffer = Graphics::buffer_resource_manager.create_buffer({ Graphics::BufferType::INDEX_BUFFER, Graphics::BufferFormat::R32_UINT, sponza.indices.size() }, sponza.indices.data());

		Graphics::Renderable obj;
		obj.u_model = glm::mat4(1.f);
		obj.u_model = glm::scale(obj.u_model, glm::vec3(0.01f, 0.01f, 0.01f));
		obj.set_vertex_data(vertex_buffer, index_buffer);

		Graphics::ResourceHandle<Graphics::Shader> vs_obj = Graphics::shader_resource_manager.get_from_file(SHADERS_PATH"default_vs.vert");
		Graphics::ResourceHandle<Graphics::Shader> ps_obj = Graphics::shader_resource_manager.get_from_file(SHADERS_PATH"default_ps.frag");
		Graphics::ShaderPipeline obj_shader_pipeline = Graphics::shader_resource_manager.create_linked_shader_pipeline(vs_obj, ps_obj);

		Graphics::Material obj_mat;
		obj_mat.init(obj_shader_pipeline);

		obj.set_material(&obj_mat);

		renderer.pass_gbuffer.add_renderable(&obj);

		while (is_running)
		{
			Time::FrameTracking::cpu_frametracking_start();
			Time::FrameTracking::gpu_frametracking_start();
			Time::update();
			poll_events(Time::get_delta_time());

			Input::InputManager::update();
			renderer.camera.update(Time::get_delta_time());

			// TODO: update logic here		

			renderer.render();

			UI::GUI::display(this);

			Time::FrameTracking::cpu_frametracking_end();
			Time::FrameTracking::gpu_frametracking_end();

			renderer.flush();
		}
	}

	void App::destroy()
	{
		renderer.destroy();
	}

}