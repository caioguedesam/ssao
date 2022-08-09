#include "stdafx.h"
#include "render/render_target.h"
#include <glad/glad.h>
#include "debugging/gl.h"

void RenderTarget::bind()
{
	GL(glBindFramebuffer(GL_FRAMEBUFFER, apiHandle));
}

void RenderTarget::unbind()
{
	GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void RenderTarget::init(uint32_t w, uint32_t h)
{
	if (apiHandle == HANDLE_INVALID)
	{
		GL(glGenFramebuffers(1, &apiHandle));
	}

	bind();

	// Create depth buffer
	GL(glGenRenderbuffers(1, &depthBufferApiHandle));
	GL(glBindRenderbuffer(GL_RENDERBUFFER, depthBufferApiHandle));
	GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h));
	GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferApiHandle));

	unbind();
}

void RenderTarget::clear(const float& r, const float& g, const float& b, const float& a)
{
	bind();
	GL(glClearColor(r, g, b, a));
	GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	unbind();
}

void RenderTarget::setOutput(ResourceHandle<Texture> textureHandle, uint32_t slot)
{
	ASSERT(textureHandle.isValid(), "Trying to set invalid texture to render target output.");
	ASSERT(slot < MAX_RENDER_OUTPUTS, "Trying to set texture to output over maximum render target outputs.");

	bind();
	targetOutputs[slot] = textureHandle;
	g_textureResourceManager.bindTexture(textureHandle, slot);
	GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, GL_TEXTURE_2D, g_textureResourceManager.get(textureHandle)->apiHandle, 0));
	updateOutputs();
	unbind();
}

void RenderTarget::updateOutputs()
{
	static GLenum colorBuffers[MAX_RENDER_OUTPUTS];
	memset(colorBuffers, 0, sizeof(colorBuffers));
	int count = 0;
	for (int i = 0; i < MAX_RENDER_OUTPUTS; i++)
	{
		if (targetOutputs[i].isValid())
		{
			colorBuffers[count] = GL_COLOR_ATTACHMENT0 + i;
			count++;
		}
	}
	GL(glDrawBuffers(count, colorBuffers));
}

bool RenderTarget::isReady()
{
	bind();
	int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	unbind();
	return status == GL_FRAMEBUFFER_COMPLETE;
}