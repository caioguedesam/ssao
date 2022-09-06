#pragma once
#include "stdafx.h"
#include "resource/resource_manager.h"
#include "render/material.h"
#include "core/hash.h"

namespace Ty
{
	namespace Graphics
	{
		struct MaterialResourceManager : ResourceManager<Material>
		{
			std::unordered_map<const char*, ResourceHandle<Material>, Hash::HashFunction_CSTR> handle_list;

			void init();
			void destroy();

			ResourceHandle<Material> create_material(const char* name, ShaderPipeline shader_pipeline);
			ResourceHandle<Material> create_material(const char* name, ShaderPipeline shader_pipeline, std::vector<ResourceHandle<Texture>>& textures);
			ResourceHandle<Material> get_material(const char* name);

			// TODO_RENDER: Add methods to manipulate material properties when supported

			void bind_material(ResourceHandle<Material> material_handle);
			void bind_texture_to_slot(ResourceHandle<Material> material_handle, ResourceHandle<Texture> texture_handle, uint32_t slot);
			void set_material_uniform(ResourceHandle<Material> material_handle, const char* bind_name, const glm::mat4& bind_value);
			void set_material_uniform(ResourceHandle<Material> material_handle, const char* bind_name, const int& bind_value);
			void set_material_uniform(ResourceHandle<Material> material_handle, const char* bind_name, const float& bind_value);
			void set_material_uniform(ResourceHandle<Material> material_handle, const char* bind_name, const glm::vec3& bind_value);
			// TODO_RENDER: Add set uniform methods
		};

		extern MaterialResourceManager material_resource_manager;
	}
}