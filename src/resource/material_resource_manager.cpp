#include "stdafx.h"
#include "resource/material_resource_manager.h"
#include "core/math.h"

namespace Ty
{
	namespace Graphics
	{
		MaterialResourceManager material_resource_manager;

		void MaterialResourceManager::init() {};
		void MaterialResourceManager::destroy() {};

		ResourceHandle<Material> MaterialResourceManager::create_material(const char* name, ShaderPipeline shader_pipeline)
		{
			ResourceHandle<Material> handle;
			ASSERT(handle_list.count(name) == 0, "[ERROR:MATERIAL] Trying to create material with already registered name.");
			Material* material = new Material();
			material->init(name, shader_pipeline);

			handle = add(material);
			handle_list[name] = handle;
			return handle;
		}

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
			handle_list[name] = handle;
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

		void MaterialResourceManager::bind_texture_to_slot(ResourceHandle<Material> material_handle, ResourceHandle<Texture> texture_handle, uint32_t slot)
		{
			bind_material(material_handle);
			get(material_handle)->add_texture_to_slot(texture_handle, slot);
		}

		void MaterialResourceManager::set_material_uniform(ResourceHandle<Material> material_handle, const char* bind_name, const Math::m4f& bind_value)
		{
			get(material_handle)->shader_pipeline.set_uniform(bind_name, bind_value);
		}

		void MaterialResourceManager::set_material_uniform(ResourceHandle<Material> material_handle, const char* bind_name, const int& bind_value)
		{
			get(material_handle)->shader_pipeline.set_uniform(bind_name, bind_value);
		}
		void MaterialResourceManager::set_material_uniform(ResourceHandle<Material> material_handle, const char* bind_name, const float& bind_value)
		{
			get(material_handle)->shader_pipeline.set_uniform(bind_name, bind_value);
		}
		void MaterialResourceManager::set_material_uniform(ResourceHandle<Material> material_handle, const char* bind_name, const Math::v3f& bind_value)
		{
			get(material_handle)->shader_pipeline.set_uniform(bind_name, bind_value);
		}
	};
};