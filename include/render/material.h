#pragma once

#include "render/shader.h"
#include "render/texture.h"
#include <vector>

class Material
{
public:
	Shader* shader;
	std::vector<Texture*> textures;

	void Init(Shader* sh);
	void AddTextureToSlot(Texture* tex, uint32_t slot);
	void Bind();
};