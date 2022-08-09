#include "stdafx.h"
#include "render/renderer.h"
#include "render/rendering_resources.h"
#include <glad/glad.h>
#include "debugging/gl.h"

// TODO_RENDER: Implement all pass methods for custom render passes (CONTINUE FROM HERE)

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

void RenderPass_SSAO::pass(Renderer* renderer)
{
	rt->bind();
	rt->clear(1, 1, 1, 1);
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
	rt->clear(1, 1, 1, 1);
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
	RenderViewport temp = renderer->renderViewport;
	RenderViewport fpsGraphViewport(FPS_WINDOW_WIDTH, FPS_WINDOW_HEIGHT, 0, 0);
	renderer->setViewport(fpsGraphViewport);
	RenderParams params;
	params.model = glm::mat4(1.f);
	params.view = renderer->camera.GetViewMatrix();
	params.proj = renderer->camera.GetProjectionMatrix();

	ui_fpsGraph.update();
	ui_fpsGraph.fpsGraphImmRenderable.draw(params);

	renderer->setViewport(temp);
	rt->unbind();
}

void RenderPass_Lighting::pass(Renderer* renderer)
{
	rt->bind();
	rt->clear();
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
	{
		setViewport();
		GL(glEnable(GL_DEPTH_TEST));
		clear(0.169f, 0.169f, 0.169f, 1.f);		// This clears background
	}

	// Render passes (customize order as needed)
	{
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
}

void Renderer::flush()
{
	SDL_GL_SwapWindow(pWindow->handle);
}