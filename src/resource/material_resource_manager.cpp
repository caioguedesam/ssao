#include "resource/material_resource_manager.h"

namespace Ty
{
	namespace Graphics
	{
		MaterialResourceManager material_resource_manager;

		void MaterialResourceManager::init() {};
		void MaterialResourceManager::destroy() {};

		ResourceHandle<Material> MaterialResourceManager::create_material(const char* name, ShaderPipeline shader_pipeline, std::vector<ResourceHandle<Texture>>& textures)
		{
			ResourceHandle<Material> handle;
			ASSERT(handle_list.count(name) == 0, "[ERROR:MATERIAL] Trying to create material with already registered name.");
			Material* material = new Material();
			material->init(name, shader_pipeline);
			for (uint32_t i = 0; i < textures.size(); i++)
			{
				material->add_texture_to_slot(textures[i], i);
			}

			handle = add(material);
			handle_list[get(handle)->name] = handle;
			return handle;
		}

		ResourceHandle<Material> MaterialResourceManager::get_material(const char* name)
		{
			ASSERT_FORMAT(handle_list.count(name) != 0, "[ERROR:MATERIAL] Trying to get unregistered material with name %s.", name);
			ResourceHandle<Material> handle = handle_list[name];
			return handle;
		}

		void MaterialResourceManager::bind_material(ResourceHandle<Material> material_handle)
		{
			get(material_handle)->bind();
		}
	};
};