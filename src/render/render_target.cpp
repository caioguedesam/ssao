#include "render/render_target.h"
#include "debugging/assert.h"
#include <iostream>


RenderTarget::RenderTarget()
{
	/*glGenFramebuffers(1, &handle);*/
}

void RenderTarget::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, handle);
}

void RenderTarget::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTarget::Init(uint32_t w, uint32_t h)
{
	if (!handle)
	{
		glGenFramebuffers(1, &handle);
	}

	Bind();

	// Create color texture and attach
	texture.Init(w, h, 4, nullptr, Texture::CreationFlags::RENDER_TARGET);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.handle, 0);

	// Create depth buffer and attach
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer status error on initialization");
	
	Unbind();
}
