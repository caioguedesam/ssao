#pragma once

#include "tiny_obj_loader.h"
#include "model/model.h"
#include "render/texture.h"

class ResourceLoader
{
public:
	static tinyobj::ObjReader meshReader;

	static void LoadModel(Model& targetModel, const char* path);
};