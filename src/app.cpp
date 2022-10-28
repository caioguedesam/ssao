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
		Graphics::ResourceHandle<Graphics::Model> model_sponza = Graphics::model_resource_manager.load_from_file(MODELS_PATH"sponza.obj");

		auto sponza_parts = Graphics::model_resource_manager.get_parts(model_sponza);
#if !USE_RAYTRACING
        // Realtime pipeline
		for (int i = 0; i < sponza_parts.size(); i++)
		{
			Graphics::MeshRenderable* part_renderable = new Graphics::MeshRenderable();
			part_renderable->set_mesh_data(sponza_parts[i].first, sponza_parts[i].second);
			part_renderable->u_model = Math::identity();
			part_renderable->u_model = Math::scale(0.01f, 0.01f, 0.01f) * part_renderable->u_model;
			part_renderable->u_model = Math::rotation(Math::to_rad(90.f), 0.f, 1.f, 0.f) * part_renderable->u_model;
        
			renderer.pass_gbuffer.add_renderable(part_renderable);
		}

		while (is_running)
		{
			Time::FrameTracking::cpu_frametracking_start();
			//Time::FrameTracking::gpu_frametracking_start();
			Time::update();
			poll_events(Time::get_delta_time());
        
			Input::InputManager::update();
			renderer.camera.update(Time::get_delta_time());
        
			// TODO: update logic here		
        
			renderer.render();
        
			UI::GUI::display(this);
        
			Time::FrameTracking::cpu_frametracking_end();
			//Time::FrameTracking::gpu_frametracking_end();
        
			renderer.flush();
		}
#else
        // Raytracing pipeline:
        u32* framebuffer = (u32*)malloc(sizeof(u32) * GAME_RENDER_WIDTH * GAME_RENDER_HEIGHT);
        u32* framebuffer_depth = (u32*)malloc(sizeof(u32) * GAME_RENDER_WIDTH * GAME_RENDER_HEIGHT);

        //      Populate MeshRenderables
        std::vector<Graphics::MeshRenderable*> renderables;
        for (int i = 0; i < sponza_parts.size(); i++)
        {
        	Graphics::MeshRenderable* part_renderable = new Graphics::MeshRenderable();
        	part_renderable->set_mesh_data(sponza_parts[i].first, sponza_parts[i].second);
        	part_renderable->u_model = Math::identity();
        	part_renderable->u_model = Math::scale(0.01f, 0.01f, 0.01f) * part_renderable->u_model;
        	part_renderable->u_model = Math::rotation(Math::to_rad(90.f), 0.f, 1.f, 0.f) * part_renderable->u_model;
            part_renderable->mesh->init_bounding_volume(part_renderable->u_model);
        
            renderables.push_back(part_renderable);
        }

        //      Generate rays from camera
        f32 scale = tan(Math::to_rad((f32)DEFAULT_CAMERA_FOV * 0.5f));
        f32 aspect = (f32)GAME_RENDER_WIDTH / (f32) GAME_RENDER_HEIGHT;
        Math::m4f camera_to_world = Math::inverse(renderer.camera.get_view_matrix());
        for(i32 i = 0; i < GAME_RENDER_HEIGHT; i++)
        {
            for(i32 j = 0; j < GAME_RENDER_WIDTH; j++)
            {
                f32 x = (2 * (j + 0.5) / (f32)GAME_RENDER_WIDTH - 1) * aspect * scale;
                f32 y = (1 - 2 * (i + 0.5) / (f32)GAME_RENDER_HEIGHT) * scale;
                Math::v3f dir = { x, y, -1 };
                dir = Math::transform(camera_to_world, dir, 0);
                dir = Math::normalize(dir);

                Math::v3f intersect = {};
                bool hit = false;
                f32 hit_len = 0;
                Math::v2f hit_uv;
                Graphics::MeshRenderable* hit_renderable = nullptr;
                for(u64 k = 0; k < renderables.size(); k++)
                {
                    //      For every renderable, check if ray collides with any of its triangles, and keep the closer one.
                    Graphics::MeshRenderable* renderable = renderables[k];
                    Graphics::Mesh* mesh = renderable->mesh;
                    if (!Math::raycast_box(renderer.camera.position, dir, mesh->aabb, nullptr)) continue;
                    
                    for(u64 l = 0; l < mesh->vertex_count; l+=3)
                    {
                        Math::v3f v0 =
                        {
                            mesh->vertex_data[l].px,
                            mesh->vertex_data[l].py,
                            mesh->vertex_data[l].pz,
                        };
                        Math::v3f v1 =
                        {
                            mesh->vertex_data[l+1].px,
                            mesh->vertex_data[l+1].py,
                            mesh->vertex_data[l+1].pz,
                        };
                        Math::v3f v2 =
                        {
                            mesh->vertex_data[l+2].px,
                            mesh->vertex_data[l+2].py,
                            mesh->vertex_data[l+2].pz,
                        };
                        v0 = Math::transform(renderable->u_model, v0, 1);
                        v1 = Math::transform(renderable->u_model, v1, 1);
                        v2 = Math::transform(renderable->u_model, v2, 1);

                        if(Math::raycast_triangle(renderer.camera.position, dir, v0, v1, v2, &intersect))
                        {
                            f32 intersect_len = Math::len(intersect - renderer.camera.position);
                            if (!hit || intersect_len < hit_len)
                            {
                                hit_len = intersect_len;
                                hit_renderable = renderable;
                                Math::v3f barycentric = Math::get_barycentric_coordinates(intersect, v0, v1, v2);
                                hit_uv.u    = barycentric.x * mesh->vertex_data[l].tx
                                    + barycentric.y * mesh->vertex_data[l+1].tx
                                    + barycentric.z * mesh->vertex_data[l+2].tx;
                                hit_uv.v    = barycentric.x * mesh->vertex_data[l].ty
                                    + barycentric.y * mesh->vertex_data[l+1].ty
                                    + barycentric.z * mesh->vertex_data[l+2].ty;
                            }
                            hit = true;
                        }
                    }
                }

                // If there's any collision, render the final color based on distance
                if(hit)
                {
                    u32 pixel;

                    // Sample renderable's texture
                    Graphics::Material* hit_material = Graphics::material_resource_manager.get(hit_renderable->material);
                    Graphics::Texture* hit_diffuse = Graphics::texture_resource_manager.get(hit_material->textures[0]);

                    Math::v2f pix = {};
                    pix.x = (hit_uv.u * hit_diffuse->desc.width) - 0.5f;
                    pix.y = ((1 - hit_uv.v) * hit_diffuse->desc.height) - 0.5f;

                    pix.x = Math::fwrap(pix.x, 0, hit_diffuse->desc.width);
                    pix.y = Math::fwrap(pix.y, 0, hit_diffuse->desc.height);

                    u32 stride = hit_diffuse->desc.format == Graphics::TextureFormat::R8_G8_B8_A8_UNORM
                        ? 4
                        : 3;

                    u32 pixel_offset = ((u32)pix.y * hit_diffuse->desc.width + (u32)pix.x) * stride;
                    u8* hit_addr = (u8*)(hit_diffuse->pData) + pixel_offset;

                    u8 r = *(hit_addr);
                    u8 g = *(hit_addr + 1);
                    u8 b = *(hit_addr + 2);
                    u8 a = 255;                 // TODO_RENDER: Account for alpha of hit pixel. This'll require sampling before.

                    pixel = (pixel & 0xFFFFFF00) |  r;
                    pixel = (pixel & 0xFFFF00FF) | ((uint32_t)g <<  8);
                    pixel = (pixel & 0xFF00FFFF) | ((uint32_t)b << 16);
                    pixel = (pixel & 0x00FFFFFF) | ((uint32_t)a << 24);
                    framebuffer[i * GAME_RENDER_WIDTH + j] = pixel;
                    
                    // Render to depth texture as well
                    f32 color_t = Math::lerp(0, 255, 1.f - (hit_len / 100.f));
                    r = (u8)color_t, g = (u8)color_t, b = (u8)color_t, a = 255;

                    pixel = (pixel & 0xFFFFFF00) |  r;
                    pixel = (pixel & 0xFFFF00FF) | ((uint32_t)g <<  8);
                    pixel = (pixel & 0xFF00FFFF) | ((uint32_t)b << 16);
                    pixel = (pixel & 0x00FFFFFF) | ((uint32_t)a << 24);
                    framebuffer_depth[i * GAME_RENDER_WIDTH + j] = pixel;
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
                    framebuffer_depth[i * GAME_RENDER_WIDTH + j] = pixel;
                }
            }
            printf("Finished pixels at row h:%d\n", i);
        }

        stbi_write_png(RESOURCES_PATH"out/raytrace_out_diffuse.png", GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT, 4, framebuffer, GAME_RENDER_WIDTH * 4);
        stbi_write_png(RESOURCES_PATH"out/raytrace_out_depth.png", GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT, 4, framebuffer_depth, GAME_RENDER_WIDTH * 4);
#endif
	}

	void App::destroy()
	{
		renderer.destroy();
	}

}