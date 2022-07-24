#include "stdafx.h"
#include "render/render_target.h"
#include "debugging/gl.h"

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

void RenderTarget::SetOutputTexture(ResourceHandle<Texture> textureHandle, uint32_t slot)
{
	Bind();
	textures[slot] = textureHandle;
	g_textureResourceManager.bindTexture(textureHandle, slot);
	GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, GL_TEXTURE_2D, g_textureResourceManager.get(textureHandle)->apiHandle, 0));
	UpdateDrawTargets();
}

void RenderTarget::UpdateDrawTargets()
{
	std::vector<GLenum> drawTargets;
	for (int i = 0; i < MAX_TEXTURE_SLOTS; i++)
	{
		if (textures[i].isValid())
		{
			drawTargets.push_back(GL_COLOR_ATTACHMENT0 + i);
		}
	}
	GL(glDrawBuffers(drawTargets.size(), &drawTargets[0]));
}

void RenderTarget::Init(uint32_t w, uint32_t h, ResourceHandle<Texture> firstTextureHandle)
{
	if (handle == HANDLE_INVALID)
	{
		GL(glGenFramebuffers(1, &handle));
		textures = std::vector<ResourceHandle<Texture>>(MAX_TEXTURE_SLOTS);
	}

	Bind();

	// Create color texture and attach
	SetOutputTexture(firstTextureHandle, 0);

	// Create depth buffer and attach
	GL(glGenRenderbuffers(1, &depthBuffer));
	GL(glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer));
	GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h));
	GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer));

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer status error on initialization");
	
	Unbind();
}
