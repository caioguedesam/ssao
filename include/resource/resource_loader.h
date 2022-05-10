#pragma once

#include "tiny_obj_loader.h"
#include "stb_image.h"
#include "model/model.h"
#include "render/texture.h"

class ResourceLoader
{
public:
	static tinyobj::ObjReader meshReader;

	static void LoadModel(Model& targetModel, const char* path);
	static void LoadTexture(Texture& targetTexture, const char* path);
};