#include "render/model.h"

namespace Ty
{
	namespace Graphics
	{
		void Model::add_new_part(Mesh* mesh, ResourceHandle<Material> material_handle)
		{
			ASSERT(mesh, "[ERROR:MODEL] Trying to add invalid mesh to model.");
			ASSERT(material_handle.is_valid(), "[ERROR:MODEL] Trying to add invalid material to model.");

			parts.push_back(std::pair<Mesh*, ResourceHandle<Material>>(mesh, material_handle));
		}
	}
}