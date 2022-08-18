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
		struct ShaderPipelineResourceManager : ResourceManager<ShaderPipeline>
		{
			std::unordered_map<
				std::tuple<ResourceHandle<Shader>, ResourceHandle<Shader>>,
				ResourceHandle<ShaderPipeline>
			> pipelinesPerShader;

			ResourceHandle<ShaderPipeline> getLinkedShaderPipeline(ResourceHandle<Shader> vs, ResourceHandle<Shader> ps, bool link = true);
			void linkPipeline(ResourceHandle<ShaderPipeline> shader_pipeline_handle);
			void bindPipeline(ResourceHandle<ShaderPipeline> shader_pipeline_handle);
		};

		struct ShaderResourceManager : ResourceManager<Shader>
		{
			std::unordered_map<FileSystem::FilePath, ResourceHandle<Shader>, FileSystem::HashFunction_FilePath> handleList;

			void init();
			void destroy();

			ResourceHandle<Shader> compileShader(const char* filePath);
			ResourceHandle<Shader> getFromFile(const char* filePath);
			void reloadShaders();

			ShaderPipeline createLinkedShaderPipeline(ResourceHandle<Shader> vs, ResourceHandle<Shader> ps);
			void linkShaders(ShaderPipeline& shaderPipeline);
			void bindShaderPipeline(ShaderPipeline& shaderPipeline);

			// TODO_MEMORY, TODO_SHADER: Shader freeing (on demand/destruction)
		};

		extern ShaderResourceManager g_shaderResourceManager;

	}
}