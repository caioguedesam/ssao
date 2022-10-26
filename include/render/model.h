#pragma once

#include "stdafx.h"
#include "resource/material_resource_manager.h"
#include "core/math.h"

namespace Ty
{
	namespace Graphics
	{
		struct MeshVertex
		{
			float px, py, pz;
			float nx, ny, nz;
			float tx, ty;
		};

		struct Mesh
		{
			MeshVertex* vertex_data;
			size_t vertex_count;
			uint32_t* index_data;
			size_t index_count;

			/*std::vector<float> vertices;
			std::vector<uint32_t> indices;*/

            Math::Box aabb;

            void init_bounding_volume();
		};

		struct Model
		{
			std::vector<std::pair<Mesh*, ResourceHandle<Material>>> parts;
			
			void add_new_part(Mesh* mesh, ResourceHandle<Material> material_handle);
		};
	}
}
