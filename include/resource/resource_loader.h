#pragma once

#include "tiny_obj_loader.h"
#include "model/model.h"
#include "render/texture.h"

namespace Ty
{
	namespace AssetSystem
	{
		class ResourceLoader
		{
		public:
			static tinyobj::ObjReader mesh_reader;

			static void load_model(Model& target_model, const char* path);
		};
	}
}