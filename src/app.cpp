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
#include "stb_image.h"


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

#if USE_RAYTRACING
    Math::v3f pixel_to_color(u32 pixel)
    {
        Math::v3f color;
        u8 r = (u32)(pixel & 0x000000FF);
        u8 g = ((u32)(pixel & 0x0000FF00) >>  8);
        u8 b = ((u32)(pixel & 0x00FF0000) >> 16);

        color.r = (f32)r / (f32)0xFF;
        color.g = (f32)g / (f32)0xFF;
        color.b = (f32)b / (f32)0xFF;

        return color;
    }

    u32 color_to_pixel(Math::v3f color)
    {
        u8 r = (u8)(color.r * 0xFF);
        u8 g = (u8)(color.g * 0xFF);
        u8 b = (u8)(color.b * 0xFF);
        u32 pixel = 0;
        pixel = (pixel & 0xFFFFFF00) |  r;
        pixel = (pixel & 0xFFFF00FF) | ((uint32_t)g <<  8);
        pixel = (pixel & 0xFF00FFFF) | ((uint32_t)b << 16);
        pixel = (pixel & 0x00FFFFFF) | ((uint32_t)255 << 24);   // TODO: Alpha support

        return pixel;
    }

    u32 get_texel_nearest(f32 u, f32 v, i32 texture_width, i32 texture_height, i32 texture_channels, u8* texture_data)
    {
        u = Math::fwrap(u, 0, 1);
        v = Math::fwrap(v, 0, 1);

        i32 pixel_x = u * texture_width;
        i32 pixel_y = v * texture_height;

        u8* hit_addr = (u8*)(texture_data) + (pixel_y * texture_width + pixel_x) * texture_channels;
        u8 r = *(hit_addr);
        u8 g = *(hit_addr + 1);
        u8 b = *(hit_addr + 2);
        //u8 a = *(hit_addr + 3);
        
        u8 a = 255;    // TODO: Alpha support

        u32 pixel = 0;
        pixel = (pixel & 0xFFFFFF00) |  r;
        pixel = (pixel & 0xFFFF00FF) | ((uint32_t)g <<  8);
        pixel = (pixel & 0xFF00FFFF) | ((uint32_t)b << 16);
        pixel = (pixel & 0x00FFFFFF) | ((uint32_t)a << 24);

        // TODO: Add this to get_texel_bilinear somehow
        if(texture_channels == 1)
        {
            pixel = (u32)*(hit_addr);
        }

        return pixel;
    }

    u32 get_texel_bilinear(f32 u, f32 v, i32 texture_width, i32 texture_height, i32 texture_channels, u8* texture_data)
    {
        u = Math::fwrap(u, 0, 1);
        v = Math::fwrap(v, 0, 1);

        f32 pixel_x = u * texture_width - 0.5f, pixel_y = v * texture_height - 0.5f;
        pixel_x = CLAMP(pixel_x, 0, texture_width   - 1);
        pixel_y = CLAMP(pixel_y, 0, texture_height  - 1);

        f32 t_x = pixel_x - (i32)floor(pixel_x);
        f32 t_y = pixel_y - (i32)floor(pixel_y);

        f32 t_pi[2] = { 1.f - t_x, t_x };
        f32 t_pj[2] = { 1.f - t_y, t_y };

        u8 r = 0, g = 0, b = 0, a = texture_channels == 4 ? 0 : 255;
        for(i32 pi = 0; pi < 2; pi++)
        {
            for(i32 pj = 0; pj < 2; pj++)
            {
                f32 p = t_pi[pi] * t_pj[pj];
                if(p)
                {
                    i32 hit_x = CLAMP((i32)pixel_x + pi, 0, texture_width);
                    i32 hit_y = CLAMP((i32)pixel_y + pj, 0, texture_height);
                    u8* hit_addr = (u8*)(texture_data) + (hit_y * texture_width + hit_x) * texture_channels;
                    r += (*(hit_addr)     ) * p;
                    g += (*(hit_addr + 1) ) * p;
                    b += (*(hit_addr + 2) ) * p;
                    if(texture_channels == 4) a += (*(hit_addr + 3) ) * p;
                }
            }
        }
        u32 pixel = 0;
        a = 255;    // TODO: Alpha support

        pixel = (pixel & 0xFFFFFF00) |  r;
        pixel = (pixel & 0xFFFF00FF) | ((uint32_t)g <<  8);
        pixel = (pixel & 0xFF00FFFF) | ((uint32_t)b << 16);
        pixel = (pixel & 0x00FFFFFF) | ((uint32_t)a << 24);

        return pixel;
    }

    struct RaycastHitInfo
    {
        bool hit = false;

        f32 distance = 0.f;
        Math::v3f position = {};
        Math::v3f normal = {};
        Math::v2f uv = {};
        Graphics::MeshRenderable* renderable = nullptr;
    };

    RaycastHitInfo get_ray_intersection(const std::vector<Graphics::MeshRenderable*>& scene_renderables, const Math::v3f& ray_origin, const Math::v3f& ray_dir, f32 max_distance = FLT_MAX)
    {
        RaycastHitInfo result = {};

        for(u64 k = 0; k < scene_renderables.size(); k++)
        {
            //  For every renderable, check if ray collides with any of its triangles, and keep the closer one.
            Graphics::MeshRenderable* renderable = scene_renderables[k];
            Graphics::Mesh* mesh = renderable->mesh;
            Math::v3f aabb_hit_position = {};
            if (!Math::raycast_box(ray_origin, ray_dir, mesh->aabb, &aabb_hit_position)) continue;   // AABB for simple acceleration
            if (Math::len(aabb_hit_position - ray_origin) > max_distance) continue;                  // Don't allow overshooting distance in AABB
                    
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

                Math::v3f intersect = {};
                if(Math::raycast_triangle(ray_origin, ray_dir, v0, v1, v2, &intersect))
                {
                    f32 intersect_len = Math::len(intersect - ray_origin);
                    if (intersect_len > max_distance) continue;     // Also don't allow overshooting max distance on narrow collision tests
                    if (!result.hit || intersect_len < result.distance)
                    {
                        RaycastHitInfo old_result = result;

                        result.hit = true;
                        result.distance = intersect_len;
                        result.renderable = renderable;
                        result.position = intersect;
                                
                        Math::v3f v0v1 = v1 - v0;
                        Math::v3f v0v2 = v2 - v0;
                        result.normal = Math::normalize(Math::cross(v0v1, v0v2));

                        Math::v3f barycentric = Math::get_barycentric_coordinates(intersect, v0, v1, v2);
                        result.uv.u    = barycentric.x * mesh->vertex_data[l].tx
                            + barycentric.y * mesh->vertex_data[l+1].tx
                            + barycentric.z * mesh->vertex_data[l+2].tx;
                        result.uv.v    = barycentric.x * mesh->vertex_data[l].ty
                            + barycentric.y * mesh->vertex_data[l+1].ty
                            + barycentric.z * mesh->vertex_data[l+2].ty;

                        // TODO: Check alpha mask here
                        if(renderable->has_alpha_mask)
                        {

                            Graphics::Material* hit_material = Graphics::material_resource_manager.get(renderable->material);
                            Graphics::Texture* hit_alpha = Graphics::texture_resource_manager.get(hit_material->textures[4]);

                            u32 channels = 1;
                            if(hit_alpha->desc.format == Graphics::TextureFormat::R8_G8_B8_A8_UNORM)
                            {
                                channels = 4;
                            }
                            else if(hit_alpha->desc.format == Graphics::TextureFormat::R8_G8_B8_UNORM)
                            {
                                channels = 3;
                            }
                            else if(hit_alpha->desc.format == Graphics::TextureFormat::R8_G8_UNORM)
                            {
                                channels = 2;
                            }
                            //u32 pixel = get_texel_bilinear(result.uv.u, 1.f - result.uv.v, hit_alpha->desc.width, hit_alpha->desc.height, channels, (u8*)hit_alpha->pData);   // Bilinear still has broken alpha, using nearest
                            u32 pixel = get_texel_nearest(result.uv.u, 1.f - result.uv.v, hit_alpha->desc.width, hit_alpha->desc.height, channels, (u8*)hit_alpha->pData);

                            // If the mask texture sample is 0, ignore this raycast (hit a transparent pixel)
                            if(pixel == 0)
                            {
                                result = old_result;
                            }
                        }
                    }
                }
            }
        }
        
        return result;
    }

    f32 get_occlusion_at_point(const std::vector<Graphics::MeshRenderable*>& scene_renderables, Math::v3f origin_position, Math::v3f origin_normal, i32 ao_samples, f32 ao_radius)
    {
        f32 ao = 0;

        for(i32 i = 0; i < ao_samples; i++)
        {
            // Random direction for occlusion sample
            Math::v3f ao_dir =
            {
                Random::dist_uniform(),
                Random::dist_uniform(),
                Random::dist_uniform(),
            };
            f32 d = Math::dot(ao_dir, origin_normal);
            if (d < 0)
            {
                ao_dir =
                {
                    -ao_dir.x,
                    -ao_dir.y,
                    -ao_dir.z,
                };
            }

            RaycastHitInfo raycast = get_ray_intersection(scene_renderables, origin_position, ao_dir, ao_radius);
            if(raycast.hit)
            {
                ao += 1.f;
            }
        }

        ao /= (f32)ao_samples;
        return ao;
    }

    struct rtao_job_params
    {
        i32 job_line_start = 0;
        i32 job_line_count = 0;
        std::vector<Graphics::MeshRenderable*>* job_renderables = nullptr;
        f32 scale = 0.f;
        f32 aspect = 0.f;
        Math::m4f camera_to_world;
        Graphics::Renderer* renderer;
        u32* framebuffer;
    };

    DWORD WINAPI rtao_job(void* args)
    {
        rtao_job_params* params = (rtao_job_params*)args;
        std::vector<Graphics::MeshRenderable*>* renderables = params->job_renderables;

        i32 job_line_end = params->job_line_start + params->job_line_count;
        f32 job_percent = 0.f;
        for(i32 i = params->job_line_start; i < job_line_end; i++)
        {
            for(i32 j = 0; j < GAME_RENDER_WIDTH; j++)
            {
                f32 x = (2 * (j + 0.5) / (f32)GAME_RENDER_WIDTH - 1) * params->aspect * params->scale;
                f32 y = (1 - 2 * (i + 0.5) / (f32)GAME_RENDER_HEIGHT) * params->scale;
                Math::v3f dir = { x, y, -1 };
                dir = Math::transform(params->camera_to_world, dir, 0);
                dir = Math::normalize(dir);

                RaycastHitInfo raycast = get_ray_intersection(*renderables, params->renderer->camera.position, dir);

                // If there's any collision, render the final color based on distance
                if(raycast.hit)
                {
                    // Sample renderable's texture
                    Graphics::Material* hit_material = Graphics::material_resource_manager.get(raycast.renderable->material);
                    Graphics::Texture* hit_diffuse = Graphics::texture_resource_manager.get(hit_material->textures[0]);

                    u32 channels = hit_diffuse->desc.format == Graphics::TextureFormat::R8_G8_B8_A8_UNORM
                        ? 4
                        : 3;
                    u32 pixel = get_texel_bilinear(raycast.uv.u, 1.f - raycast.uv.v, hit_diffuse->desc.width, hit_diffuse->desc.height, channels, (u8*)hit_diffuse->pData);

                    // Calculate light contributions
                    Math::v3f hit_color = pixel_to_color(pixel);
                    Math::v3f result_color = {};
                    for(i32 l = 0; l < params->renderer->pass_lighting.light_count; l++)
                    {
                        Graphics::Light* light = &params->renderer->pass_lighting.point_lights[l];
                        Math::v3f to_light = Math::normalize(light->position - raycast.position);
                        f32 LdotN = Math::dot(raycast.normal, to_light);
                        LdotN = CLAMP(LdotN, 0, 1);
                        result_color = result_color + (hit_color * LdotN * light->power / Math::sqrlen(to_light));
                    }

                    // Add ambient occlusion
                    f32 ao = 1.f - get_occlusion_at_point(*renderables, raycast.position, raycast.normal, params->renderer->pass_ssao.ssao_data.sample_amount, params->renderer->pass_ssao.ssao_data.sample_radius);
                    ao = CLAMP(ao, 0, 1);
                    result_color = result_color * ao;

                    pixel = color_to_pixel(result_color);
                    params->framebuffer[i * GAME_RENDER_WIDTH + j] = pixel;
                }
                else
                {
                    u32 pixel;
                    u8 r = 0, g = 0, b = 0, a = 255;
                    pixel = (pixel & 0xFFFFFF00) |  r;
                    pixel = (pixel & 0xFFFF00FF) | ((uint32_t)g <<  8);
                    pixel = (pixel & 0xFF00FFFF) | ((uint32_t)b << 16);
                    pixel = (pixel & 0x00FFFFFF) | ((uint32_t)a << 24);
                    params->framebuffer[i * GAME_RENDER_WIDTH + j] = pixel;
                }
            }
            job_percent += 1.f / (f32)params->job_line_count;
            printf("[JOB %d: %.2f\%] Finished pixels at row h:%d\n", params->job_line_start / params->job_line_count, job_percent * 100.f, i);
            stbi_write_png(RESOURCES_PATH"out/raytrace_out_diffuse_bilinear.png", GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT, 4, params->framebuffer, GAME_RENDER_WIDTH * 4);
        }

        return 0;
    }

#endif

	void App::run()
	{
        // Add default first light
        renderer.pass_lighting.light_count++;
        renderer.pass_lighting.point_lights[renderer.pass_lighting.light_count - 1] = {};
        renderer.pass_lighting.point_lights[renderer.pass_lighting.light_count - 1].position.y = 1.5f;
        renderer.pass_ssao.enabled = false;

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

        //      Spawn ray tracing jobs for image sections
#define RTAO_JOB_COUNT 4
        HANDLE rtao_job_handles[RTAO_JOB_COUNT];
        rtao_job_params job_params[RTAO_JOB_COUNT];
        i32 lines_per_job = GAME_RENDER_HEIGHT / RTAO_JOB_COUNT;
        for(i32 i = 0; i < RTAO_JOB_COUNT; i++)
        {
            job_params[i].renderer = &renderer;
            job_params[i].framebuffer = framebuffer;
            job_params[i].job_renderables = &renderables;
            job_params[i].scale = scale;
            job_params[i].aspect = aspect;
            job_params[i].camera_to_world = camera_to_world;
            job_params[i].job_line_start = i * lines_per_job;
            job_params[i].job_line_count = lines_per_job;
            if(i == RTAO_JOB_COUNT - 1)
            {
                job_params[i].job_line_count = GAME_RENDER_HEIGHT - i * lines_per_job;
            }

            rtao_job_handles[i] = CreateThread(NULL, 0, rtao_job, (void*)(&job_params[i]), 0, NULL);
        }

        Time::Timer rtao_timer;
        rtao_timer.start();

        WaitForMultipleObjects(RTAO_JOB_COUNT, rtao_job_handles, TRUE, INFINITE);

        rtao_timer.end();
        printf("Finished work in %.2f seconds, with %d threads.\n", rtao_timer.elapsed_secs(), RTAO_JOB_COUNT);

        stbi_write_png(RESOURCES_PATH"out/raytrace_out_diffuse_bilinear.png", GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT, 4, framebuffer, GAME_RENDER_WIDTH * 4);
#endif
	}

	void App::destroy()
	{
		renderer.destroy();
	}

}