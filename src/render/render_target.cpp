#include "render/render_target.h"
#include "debugging/assert.h"
#include "debugging/gl.h"
#include <iostream>

#define MAX_TEXTURE_SLOTS 16	// Change this in the future if needed

RenderTarget::RenderTarget()
{

}

void RenderTarget::Bind()
{
	GL(glBindFramebuffer(GL_FRAMEBUFFER, handle));
}

void RenderTarget::Unbind()
{
	GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void RenderTarget::AddTextureToSlot(Texture* tex, uint32_t slot)
{
	Bind();
	textures[slot] = tex;
	GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, GL_TEXTURE_2D, tex->handle, 0));
}

void RenderTarget::Init(uint32_t w, uint32_t h, Texture* firstTex)
{
	if (!handle)
	{
		GL(glGenFramebuffers(1, &handle));
		textures = std::vector<Texture*>(MAX_TEXTURE_SLOTS, nullptr);
	}

	Bind();

	// Create color texture and attach
	//texture.Init(w, h, 4, nullptr, Texture::CreationFlags::RENDER_TARGET);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, firstTex->handle, 0);
	AddTextureToSlot(firstTex, 0);

	// Create depth buffer and attach
	GL(glGenRenderbuffers(1, &depthBuffer));
	GL(glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer));
	GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h));
	GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer));

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer status error on initialization");
	
	Unbind();
}
