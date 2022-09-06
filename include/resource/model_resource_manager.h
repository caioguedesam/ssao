#pragma once
#include "resource/resource_manager.h"
#include "render/model.h"
#include "file/file_reader.h"
#include "core/hash.h"

namespace Ty
{
	namespace Graphics
	{
		struct ModelResourceManager : ResourceManager<Model>
		{
			std::unordered_map<FileSystem::FilePath, ResourceHandle<Model>, FileSystem::HashFunction_FilePath> handle_list;
			ShaderPipeline gbuffer_shader_pipeline;

			void init();
			void destroy();

			ResourceHandle<Model> load_from_file(const char* file_path);
			ResourceHandle<Model> get_from_file(const char* file_path);
			std::vector<std::pair<Mesh*, ResourceHandle<Material>>>& get_parts(ResourceHandle<Model> model_handle);
		};

		extern ModelResourceManager model_resource_manager;
	}
}