#pragma once

#include <stdint.h>
#include <vector>
#include <glad/glad.h>

#include "render/texture.h"

// TODO: Free resource
class RenderTarget
{
public:
	uint32_t handle;
	//Texture texture;
	std::vector<Texture*> textures;
	uint32_t depthBuffer;	// Should this be a proper resource?

	RenderTarget();

	void Bind();
	void Unbind();

	void Init(uint32_t w, uint32_t h, Texture* firstTex);
	void SetOutputTexture(Texture* tex, uint32_t slot);
	void UpdateDrawTargets();
};
