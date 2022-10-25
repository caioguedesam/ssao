#include "stdafx.h"
#include "app.h"

#include <SDL.h>

#include "time/time.h"
#include "resource/buffer_resource_manager.h"
#include "resource/texture_resource_manager.h"
#include "resource/shader_resource_manager.h"
#include "resource/material_resource_manager.h"
#include "resource/model_resource_manager.h"
#include "gui/gui.h"
#include "core/math.h"

// For writing raytraced output:
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


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
		Graphics::buffer_resource_manager.init();
		Graphics::texture_resource_manager.init();
		Graphics::shader_resource_manager.init();
		Graphics::material_resource_manager.init();
		Graphics::model_resource_manager.init();

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
				case SDLK_1:
				{
					UI::g_gui_state.show_sidebar = !UI::g_gui_state.show_sidebar;
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
//		Graphics::ResourceHandle<Graphics::Model> model_sponza = Graphics::model_resource_manager.load_from_file(MODELS_PATH"sponza.obj");
//
//		auto sponza_parts = Graphics::model_resource_manager.get_parts(model_sponza);
//		for (int i = 0; i < sponza_parts.size(); i++)
//		{
//			Graphics::MeshRenderable* part_renderable = new Graphics::MeshRenderable();
//			part_renderable->set_mesh_data(sponza_parts[i].first, sponza_parts[i].second);
//			part_renderable->u_model = Math::identity();
//			part_renderable->u_model = Math::scale(0.01f, 0.01f, 0.01f) * part_renderable->u_model;
//			part_renderable->u_model = Math::rotation(Math::to_rad(90.f), 0.f, 1.f, 0.f) * part_renderable->u_model;
//
//			renderer.pass_gbuffer.add_renderable(part_renderable);
//		}

        //// Realtime pipeline
		//while (is_running)
		//{
		//	Time::FrameTracking::cpu_frametracking_start();
		//	//Time::FrameTracking::gpu_frametracking_start();
		//	Time::update();
		//	poll_events(Time::get_delta_time());
        //
		//	Input::InputManager::update();
		//	renderer.camera.update(Time::get_delta_time());
        //
		//	// TODO: update logic here		
        //
		//	renderer.render();
        //
		//	UI::GUI::display(this);
        //
		//	Time::FrameTracking::cpu_frametracking_end();
		//	//Time::FrameTracking::gpu_frametracking_end();
        //
		//	renderer.flush();
		//}

        // Raytracing pipeline:
        u32* framebuffer = (u32*)malloc(sizeof(u32) * GAME_RENDER_WIDTH * GAME_RENDER_HEIGHT);

        //      Generate rays from camera
        f32 scale = tan(Math::to_rad((f32)DEFAULT_CAMERA_FOV * 0.5f));
        f32 aspect = (f32)GAME_RENDER_WIDTH / (f32) GAME_RENDER_HEIGHT;
        Math::m4f camera_to_world = Math::inverse(renderer.camera.get_view_matrix());
        //Math::Sphere sphere;
        //sphere.center = { 0, 2, -10.f };
        //sphere.radius = 2.f;
        Math::v3f triangle[3] =
        {
            -2.f, 2.f, -5.f,
            0, 2.f, -5.f,
            -2.f, 5.f, -5.f,
        };
        for(i32 i = 0; i < GAME_RENDER_HEIGHT; i++)
        {
            for(i32 j = 0; j < GAME_RENDER_WIDTH; j++)
            {
                f32 x = (2 * (j + 0.5) / (f32)GAME_RENDER_WIDTH - 1) * aspect * scale;
                f32 y = (1 - 2 * (i + 0.5) / (f32)GAME_RENDER_HEIGHT) * scale;
                Math::v3f dir = { x, y, -1 };
                dir = camera_to_world * dir;
                dir = Math::normalize(dir);

                // TEST: Collide rays with sphere, save output to buffer
                Math::v3f intersect = {};
                //if(Math::raycast_sphere(renderer.camera.position, dir, sphere, &intersect))
                if(Math::raycast_triangle(renderer.camera.position, dir, triangle[0], triangle[1], triangle[2], &intersect))
                {
                    u32 pixel;
                    u8 r = 0, g = 0, b = 255, a = 255;
                    pixel = (pixel & 0xFFFFFF00) |  r;
                    pixel = (pixel & 0xFFFF00FF) | ((uint32_t)g <<  8);
                    pixel = (pixel & 0xFF00FFFF) | ((uint32_t)b << 16);
                    pixel = (pixel & 0x00FFFFFF) | ((uint32_t)a << 24);
                    framebuffer[i * GAME_RENDER_WIDTH + j] = pixel;
                }
                else
                {
                    u32 pixel;
                    u8 r = 0, g = 0, b = 0, a = 255;
                    pixel = (pixel & 0xFFFFFF00) |  r;
                    pixel = (pixel & 0xFFFF00FF) | ((uint32_t)g <<  8);
                    pixel = (pixel & 0xFF00FFFF) | ((uint32_t)b << 16);
                    pixel = (pixel & 0x00FFFFFF) | ((uint32_t)a << 24);
                    framebuffer[i * GAME_RENDER_WIDTH + j] = pixel;
                }
            }
        }

        stbi_write_png(RESOURCES_PATH"out/raytrace_out.png", GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT, 4, framebuffer, GAME_RENDER_WIDTH * 4);
	}

	void App::destroy()
	{
		renderer.destroy();
	}

}