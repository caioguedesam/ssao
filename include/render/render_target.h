#pragma once

#include "stdafx.h"
#include <glad/glad.h>

#include "resource/texture_resource_manager.h"

// TODO: Free resource
class RenderTarget
{
public:
	uint32_t handle;
	std::vector<ResourceHandle<Texture>> textures;
	uint32_t depthBuffer;	// Should this be a proper resource?

	RenderTarget();

	void Bind();
	void Unbind();

	void Init(uint32_t w, uint32_t h, ResourceHandle<Texture> firstTextureHandle);
	void SetOutputTexture(ResourceHandle<Texture> textureHandle, uint32_t slot);
	void UpdateDrawTargets();
};
