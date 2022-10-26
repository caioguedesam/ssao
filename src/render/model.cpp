#include "stdafx.h"
#include "render/model.h"

namespace Ty
{
	namespace Graphics
	{
        void Mesh::init_bounding_volume()
        {
            Math::Box result = {};
            MeshVertex vertex = vertex_data[0];
            Math::v3f v = { vertex.px, vertex.py, vertex.pz };
            result.min = v;
            result.max = v;

            for(size_t i = 1; i < vertex_count; i++)
            {
                vertex = vertex_data[i];
                v = { vertex.px, vertex.py, vertex.pz };
                result = Math::box_union(result, v);
            }

            this->aabb = result;
        }

		void Model::add_new_part(Mesh* mesh, ResourceHandle<Material> material_handle)
		{
			ASSERT(mesh, "[ERROR:MODEL] Trying to add invalid mesh to model.");
			ASSERT(material_handle.is_valid(), "[ERROR:MODEL] Trying to add invalid material to model.");

			parts.push_back(std::pair<Mesh*, ResourceHandle<Material>>(mesh, material_handle));
		}
	}
}