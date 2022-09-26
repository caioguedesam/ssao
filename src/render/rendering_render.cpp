#include "stdafx.h"
#include "render/renderer.h"
#include "render/rendering_resources.h"
#include "resource/material_resource_manager.h"
#include <glad/glad.h>
#include "core/math.h"
#include "random/random.h"
#include "debugging/gl.h"

namespace Ty
{
	namespace Graphics
	{
		void RenderPass_GBuffer::add_renderable(Renderable* renderable)
		{
			ASSERT(renderable, "Trying to add null renderable to gbuffer pass.");
			renderables.push_back(renderable);
		}

		void RenderPass_GBuffer::pass(Renderer* renderer)
		{
			rt->bind();
			rt->clear();
			RenderParams params;
			params.view = renderer->camera.get_view_matrix();
			params.proj = renderer->camera.get_projection_matrix();
			for (int i = 0; i < renderables.size(); i++)
			{
				params.model = renderables[i]->u_model;
				renderables[i]->draw(params);
			}

			rt->update_output_mips();
			rt->unbind();
		}

		void RenderPass_SSAO::generate_sample_kernel()
		{
			for (int i = 0; i < ssao_data.sample_amount; i++)	// 64 points for kernel
			{
				/*glm::vec3 sample(
					Random::dist_uniform(-1.f, 1.f),
					Random::dist_uniform(-1.f, 1.f),
					Random::dist_uniform(0.f, 1.f)
				);
				sample = glm::normalize(sample);*/
				Math::v3f sample =
				{
					Random::dist_uniform(-1.f, 1.f),
					Random::dist_uniform(-1.f, 1.f),
					Random::dist_uniform(0.f, 1.f)
				};
				sample = Random::dist_uniform() * sample;
				// Push sample towards center
				float scale = float(i) / 64.f;
				scale = Math::lerp(0.1f, 1.f, scale * scale);
				sample = scale * sample;

				ssao_data.sample_kernel[i] = sample;
			}
		}

		void RenderPass_SSAO::bind_sample_kernel()
		{
			/*ssao_material.shader_pipeline.bind();*/
			material_resource_manager.bind_material(ssao_material);
			char bind_name[32];
			for (int i = 0; i < ssao_data.sample_amount; i++)
			{
				sprintf(bind_name, "samples[%d]", i);
				//ssao_material.shader_pipeline.set_uniform(bind_name, ssao_data.sample_kernel[i]);
				material_resource_manager.set_material_uniform(ssao_material, bind_name, ssao_data.sample_kernel[i]);
			}
			//ssao_material.shader_pipeline.set_uniform("kernelSize", ssao_data.sample_amount);
			material_resource_manager.set_material_uniform(ssao_material, "kernelSize", ssao_data.sample_amount);
		}

		void RenderPass_SSAO::generate_white_noise()
		{
			for (int i = 0; i < ssao_data.white_noise_dimension * ssao_data.white_noise_dimension; i++)
			{
				/*glm::vec3 noise(
					Random::dist_uniform(-1.f, 1.f),
					Random::dist_uniform(-1.f, 1.f),
					0.f
				);*/
				Math::v3f noise =
				{
					Random::dist_uniform(-1.f, 1.f),
					Random::dist_uniform(-1.f, 1.f),
					0.f
				};
				ssao_data.white_noise[i] = noise;
			}
		}

		void RenderPass_SSAO::bind_white_noise_texture()
		{
			//ssao_material.shader_pipeline.bind();
			material_resource_manager.bind_material(ssao_material);
			//ssao_material.shader_pipeline.set_uniform("noiseDimension", ssao_data.white_noise_dimension);
			material_resource_manager.set_material_uniform(ssao_material, "noiseDimension", ssao_data.white_noise_dimension);
			texture_resource_manager.update_texture(ssao_noise_texture,
				{
					static_cast<uint32_t>(ssao_data.white_noise_dimension),
					static_cast<uint32_t>(ssao_data.white_noise_dimension),
					TextureFormat::R32_G32_B32_FLOAT
				},
				&ssao_data.white_noise[0]);
		}

		void RenderPass_SSAO::bind_sample_radius()
		{
			/*ssao_material.shader_pipeline.bind();
			ssao_material.shader_pipeline.set_uniform("radius", ssao_data.sample_radius);*/
			material_resource_manager.bind_material(ssao_material);
			material_resource_manager.set_material_uniform(ssao_material, "radius", ssao_data.sample_radius);
		}

		void RenderPass_SSAO::pass(Renderer* renderer)
		{
			rt->bind();
			rt->clear();
			GL(glDisable(GL_DEPTH_TEST));
			RenderParams params;
			params.model = Math::identity();
			params.view = renderer->camera.get_view_matrix();
			params.proj = renderer->camera.get_projection_matrix();

			//renderable_screen_quad.set_material(&ssao_material);
			renderable_screen_quad.set_material(ssao_material);
			renderable_screen_quad.draw(params);

			rt->update_output_mips();
			rt->unbind();
		}

		void RenderPass_Blur::pass(Renderer* renderer)
		{
			rt->bind();
			rt->clear();
			GL(glDisable(GL_DEPTH_TEST));
			RenderParams params;
			params.model = Math::identity();
			params.view = renderer->camera.get_view_matrix();
			params.proj = renderer->camera.get_projection_matrix();

			//renderable_screen_quad.set_material(&blur_material);
			renderable_screen_quad.set_material(blur_material);
			renderable_screen_quad.draw(params);

			rt->update_output_mips();
			rt->unbind();
		}

		void RenderPass_UI::pass(Renderer* renderer)
		{
			rt->bind();
			rt->clear();
			Math::Primitives::u32_rect viewport_old = renderer->render_viewport;
			renderer->set_viewport({ FPS_WINDOW_WIDTH, FPS_WINDOW_HEIGHT, 0, 0 });
			RenderParams params;
			params.model = Math::identity();
			params.view = renderer->camera.get_view_matrix();
			params.proj = renderer->camera.get_projection_matrix();

			ui_fps_graph.update();
			ui_fps_graph.fps_graph_renderable.draw(params);

			rt->update_output_mips();
			renderer->set_viewport(viewport_old);
			rt->unbind();
		}

		void RenderPass_Lighting::pass(Renderer* renderer)
		{
			rt->bind();
			rt->clear();
			RenderParams params;
			params.model = Math::identity();
			params.view = renderer->camera.get_view_matrix();
			params.proj = renderer->camera.get_projection_matrix();

			material_resource_manager.set_material_uniform(lighting_material, "light_count", light_count);

			for (int i = 0; i < light_count; i++)
			{
				char bind_name[256];
				sprintf(bind_name, "lights[%d].position", i);
				material_resource_manager.set_material_uniform(lighting_material, bind_name, point_lights[i].position);
				sprintf(bind_name, "lights[%d].color", i);
				material_resource_manager.set_material_uniform(lighting_material, bind_name, point_lights[i].color);
				sprintf(bind_name, "lights[%d].power", i);
				material_resource_manager.set_material_uniform(lighting_material, bind_name, point_lights[i].power);
			}

			renderable_screen_quad.set_material(lighting_material);
			renderable_screen_quad.draw(params);

			rt->update_output_mips();
			rt->unbind();
		}

		void Renderer::clear(const float& r, const float& g, const float& b, const float& a)
		{
			// Clears the default framebuffer
			GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
			GL(glClearColor(r, g, b, a));
			GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		}

		void Renderer::render()
		{
			// Pre render configuration

			Math::Primitives::u32_rect default_viewport = render_viewport;
			GL(glEnable(GL_DEPTH_TEST));
			clear(0.169f, 0.169f, 0.169f, 1.f);		// This clears background

			// Render passes (customize order as needed)
			{
				set_viewport({ GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT, 0, 0 });
				// G-BUFFER
				//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// TODO_RENDER: Wireframe mode
				pass_gbuffer.pass(this);
				//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// TODO_RENDER: Wireframe mode
				// SSAO
				if (pass_ssao.enabled)
				{
					pass_ssao.pass(this);

					// BLUR
					if (pass_blur.enabled)
					{
						pass_blur.set_input_texture(pass_ssao.ssao_output_texture);
						pass_blur.pass(this);
					}
				}

				// DEBUG UI
				pass_ui.pass(this);

				// LIGHTING (FINAL)
				material_resource_manager.bind_material(pass_lighting.lighting_material);
				if (pass_ssao.enabled)
				{
					if (pass_blur.enabled)
					{
						pass_lighting.set_input_texture(pass_blur.blur_output_texture);
					}
					else
					{
						pass_lighting.set_input_texture(pass_ssao.ssao_output_texture);
					}
					material_resource_manager.set_material_uniform(pass_lighting.lighting_material, "use_ssao", true);
				}
				else
				{
					material_resource_manager.set_material_uniform(pass_lighting.lighting_material, "use_ssao", false);
				}
				pass_lighting.pass(this);
			}

			// Post render configuration
			set_viewport(default_viewport);
		}

		void Renderer::flush()
		{
			SDL_GL_SwapWindow(window->handle);
		}
	}
}