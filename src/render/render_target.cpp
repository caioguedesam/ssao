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

void RenderTarget::SetOutputTexture(Texture* tex, uint32_t slot)
{
	Bind();
	textures[slot] = tex;
	tex->Bind(slot);
	GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, GL_TEXTURE_2D, tex->handle, 0));
	UpdateDrawTargets();
}

void RenderTarget::UpdateDrawTargets()
{
	std::vector<GLenum> drawTargets;
	for (int i = 0; i < MAX_TEXTURE_SLOTS; i++)
	{
		if (textures[i] != nullptr)
		{
			drawTargets.push_back(GL_COLOR_ATTACHMENT0 + i);
		}
	}
	GL(glDrawBuffers(drawTargets.size(), &drawTargets[0]));
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
	SetOutputTexture(firstTex, 0);

	// Create depth buffer and attach
	GL(glGenRenderbuffers(1, &depthBuffer));
	GL(glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer));
	GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h));
	GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer));

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer status error on initialization");
	
	Unbind();
}
