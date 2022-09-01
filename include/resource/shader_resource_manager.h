#pragma once
#include "stdafx.h"
#include "resource/resource_manager.h"
#include "file/file_reader.h"
#include "render/shader.h"
#include "core/hash.h"

namespace Ty
{
	namespace Graphics
	{
		struct ShaderResourceManager : ResourceManager<Shader>
		{
			std::unordered_map<FileSystem::FilePath, ResourceHandle<Shader>, FileSystem::HashFunction_FilePath> handle_list;

			void init();
			void destroy();

			ResourceHandle<Shader> compile_shader(const char* file_path);
			ResourceHandle<Shader> get_from_file(const char* file_path);

			ShaderPipeline create_linked_shader_pipeline(ResourceHandle<Shader> vs, ResourceHandle<Shader> ps);
			void link_shaders(ShaderPipeline& shader_pipeline);
			void bind_shader_pipeline(ShaderPipeline& shader_pipeline);

			// TODO_MEMORY, TODO_SHADER: Shader freeing (on demand/destruction)
		};

		extern ShaderResourceManager shader_resource_manager;

	}
}