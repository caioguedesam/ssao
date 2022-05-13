#include "render/material.h"

#define MAX_TEXTURE_SLOTS 16	// Change this in the future if needed

void Material::Init(Shader* sh)
{
	shader = sh;
	textures = std::vector<Texture*>(MAX_TEXTURE_SLOTS);
}

void Material::AddTextureToSlot(Texture* tex, uint32_t slot)
{
	textures[slot] = tex;
}

void Material::Bind()
{
	shader->Bind();
	for (uint32_t i = 0; i < MAX_TEXTURE_SLOTS; i++)
	{
		if (textures[i])
		{
			// TODO: Support multiple texture dimensions
			textures[i]->Bind(i);
		}
	}
}