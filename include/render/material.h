#pragma once
#include "stdafx.h"

#include "render/shader.h"
#include "render/texture.h"

class Material
{
public:
	Shader* shader;
	std::vector<Texture*> textures;

	void Init(Shader* sh);
	void AddTextureToSlot(Texture* tex, uint32_t slot);
	void Bind();
};