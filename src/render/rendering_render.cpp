#include "stdafx.h"
#include "render/renderer.h"
#include "render/rendering_resources.h"
#include <glad/glad.h>
#include "math/math.h"
#include "random/random.h"
#include "debugging/gl.h"

namespace Ty
{
	namespace Graphics
	{
		void RenderPass_GBuffer::addRenderable(Renderable* renderable)
		{
			ASSERT(renderable, "Trying to add null renderable to gbuffer pass.");
			renderables.push_back(renderable);
		}

		void RenderPass_GBuffer::pass(Renderer* renderer)
		{
			rt->bind();
			rt->clear();
			RenderParams params;
			params.view = renderer->camera.GetViewMatrix();
			params.proj = renderer->camera.GetProjectionMatrix();
			for (int i = 0; i < renderables.size(); i++)
			{
				params.model = renderables[i]->uModel;
				renderables[i]->draw(params);
			}

			rt->unbind();
		}

		void RenderPass_SSAO::generateKernel()
		{
			for (int i = 0; i < ssao_data.ssaoKernelSize; i++)	// 64 points for kernel
			{
				glm::vec3 sample(
					Random::UniformDistribution(-1.f, 1.f),
					Random::UniformDistribution(-1.f, 1.f),
					Random::UniformDistribution(0.f, 1.f)
				);
				sample = glm::normalize(sample);
				sample *= Random::UniformDistribution();
				// Push sample towards center
				float scale = float(i) / 64.f;
				scale = Math::Lerp(0.1f, 1.f, scale * scale);
				sample *= scale;

				ssao_data.ssaoKernel[i] = sample;
			}
		}

		void RenderPass_SSAO::bindKernel()
		{
			ssao_material.shaderPipeline.bind();
			char ssaoKernelName[32];
			for (int i = 0; i < ssao_data.ssaoKernelSize; i++)
			{
				sprintf(ssaoKernelName, "samples[%d]", i);
				ssao_material.shaderPipeline.setUniform(ssaoKernelName, ssao_data.ssaoKernel[i]);
			}
			ssao_material.shaderPipeline.setUniform("kernelSize", ssao_data.ssaoKernelSize);
		}

		void RenderPass_SSAO::generateNoise()
		{
			for (int i = 0; i < ssao_data.ssaoNoiseDimension * ssao_data.ssaoNoiseDimension; i++)
			{
				glm::vec3 noise(
					Random::UniformDistribution(-1.f, 1.f),
					Random::UniformDistribution(-1.f, 1.f),
					0.f
				);
				ssao_data.ssaoNoise[i] = noise;
			}
		}

		void RenderPass_SSAO::bindNoiseTexture()
		{
			ssao_material.shaderPipeline.bind();
			ssao_material.shaderPipeline.setUniform("noiseDimension", ssao_data.ssaoNoiseDimension);
			g_textureResourceManager.updateTexture(ssao_noiseTexture,
				{
					static_cast<uint32_t>(ssao_data.ssaoNoiseDimension),
					static_cast<uint32_t>(ssao_data.ssaoNoiseDimension),
					TextureFormat::R32_G32_B32_FLOAT
				},
				&ssao_data.ssaoNoise[0]);
		}

		void RenderPass_SSAO::bindRadius()
		{
			ssao_material.shaderPipeline.bind();
			ssao_material.shaderPipeline.setUniform("radius", ssao_data.ssaoRadius);
		}

		void RenderPass_SSAO::pass(Renderer* renderer)
		{
			rt->bind();
			GL(glDisable(GL_DEPTH_TEST));
			RenderParams params;
			params.model = glm::mat4(1.f);
			params.view = renderer->camera.GetViewMatrix();
			params.proj = renderer->camera.GetProjectionMatrix();

			g_renderableScreenQuad.setMaterial(&ssao_material);
			g_renderableScreenQuad.draw(params);

			rt->unbind();
		}

		void RenderPass_Blur::pass(Renderer* renderer)
		{
			rt->bind();
			GL(glDisable(GL_DEPTH_TEST));
			RenderParams params;
			params.model = glm::mat4(1.f);
			params.view = renderer->camera.GetViewMatrix();
			params.proj = renderer->camera.GetProjectionMatrix();

			g_renderableScreenQuad.setMaterial(&blur_material);
			g_renderableScreenQuad.draw(params);

			rt->unbind();
		}

		void RenderPass_UI::pass(Renderer* renderer)
		{
			rt->bind();
			rt->clear();
			Math::Primitives::u32_rect viewport_old = renderer->renderViewport;
			renderer->setViewport({ FPS_WINDOW_WIDTH, FPS_WINDOW_HEIGHT, 0, 0 });
			RenderParams params;
			params.model = glm::mat4(1.f);
			params.view = renderer->camera.GetViewMatrix();
			params.proj = renderer->camera.GetProjectionMatrix();

			ui_fpsGraph.update();
			ui_fpsGraph.fpsGraphImmRenderable.draw(params);

			renderer->setViewport(viewport_old);
			rt->unbind();
		}

		void RenderPass_Lighting::pass(Renderer* renderer)
		{
			rt->bind();
			RenderParams params;
			params.model = glm::mat4(1.f);
			params.view = renderer->camera.GetViewMatrix();
			params.proj = renderer->camera.GetProjectionMatrix();

			g_renderableScreenQuad.setMaterial(&lighting_material);
			g_renderableScreenQuad.draw(params);

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

			Math::Primitives::u32_rect defaultViewport = renderViewport;
			GL(glEnable(GL_DEPTH_TEST));
			clear(0.169f, 0.169f, 0.169f, 1.f);		// This clears background


			// Render passes (customize order as needed)
			{
				setViewport({ GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT, 0, 0 });
				// G-BUFFER
				pass_gBuffer.pass(this);

				// SSAO
				pass_ssao.pass(this);

				// BLUR
				if (pass_blur.enabled)
				{
					pass_blur.setInputTexture(pass_ssao.ssao_outputTexture);
					pass_blur.pass(this);
				}

				// DEBUG UI
				pass_ui.pass(this);

				// LIGHTING (FINAL)
				if (pass_blur.enabled)
				{
					pass_lighting.setInputTexture(pass_blur.blur_outputTexture);
				}
				else
				{
					pass_lighting.setInputTexture(pass_ssao.ssao_outputTexture);
				}
				pass_lighting.pass(this);
			}

			// Post render configuration
			setViewport(defaultViewport);
		}

		void Renderer::flush()
		{
			SDL_GL_SwapWindow(pWindow->handle);
		}
	}
}