#include "stdafx.h"
#include "resource/shader_resource_manager.h"
#include "file/file_reader.h"
#include "debugging/gl.h"
#include "glad/glad.h"
#include "globals.h"

#define SHADER_SOURCE_MAX_SIZE 4096

namespace Ty
{
	namespace Graphics
	{
		ShaderResourceManager g_shaderResourceManager;

		ResourceHandle<ShaderPipeline> ShaderPipelineResourceManager::getLinkedShaderPipeline(ResourceHandle<Shader> vs, ResourceHandle<Shader> ps, bool link)
		{
			auto key = std::tuple<ResourceHandle<Shader>, ResourceHandle<Shader>>(vs, ps);
			ResourceHandle<ShaderPipeline> handle;
			if (pipelinesPerShader.count(key))
			{
				handle = pipelinesPerShader[key];
			}
			else
			{
				ShaderPipeline* pipeline = new ShaderPipeline(vs, ps);
				add(pipeline);
			}
			if (link)
			{
				linkPipeline(handle);
			}

			pipelinesPerShader[key] = handle;
			return handle;
		}

		void ShaderResourceManager::init()
		{
			// Fetch and compile all shaders
			std::vector<FileSystem::FilePath> files = FileSystem::FileReader::getFileNamesFromPath(SHADERS_PATH);
			for (int i = 0; i < files.size(); i++)
			{
				char path[MAX_PATH] = "";
				strcat(path, SHADERS_PATH);
				strcat(path, files[i].path);
				compileShader(path);
			}

			// TODO_SHADER: Set up shader hot reload again
		}

		void ShaderResourceManager::destroy() {}

		ResourceHandle<Shader> ShaderResourceManager::compileShader(const char* filePath)
		{
			ResourceHandle<Shader> handle;
			if (handleList.count(filePath))
			{
				handle = handleList[filePath];
			}
			else
			{
				Shader* shader = new Shader();
				handle = add(shader);
			}

			ShaderType type;
			GLenum glType;
			if (strstr(filePath, ".vert"))
			{
				type = ShaderType::VERTEX;
				glType = GL_VERTEX_SHADER;
			}
			else if (strstr(filePath, ".frag"))
			{
				type = ShaderType::PIXEL;
				glType = GL_FRAGMENT_SHADER;
			}

			uint32_t apiHandle = get(handle)->apiHandle;
			if (apiHandle == HANDLE_INVALID)
			{
				GL(apiHandle = glCreateShader(glType));
			}

			char shaderSource[SHADER_SOURCE_MAX_SIZE];
			FileSystem::FileReader::ReadFile(filePath, shaderSource);
			const char* shaderSource_cstr = shaderSource;

			GL(glShaderSource(apiHandle, 1, &shaderSource_cstr, NULL));
			GL(glCompileShader(apiHandle));

			int ret = 0;
			GL(glGetShaderiv(apiHandle, GL_COMPILE_STATUS, &ret));
			if (!ret)
			{
				char infoLog[2048];
				GL(glGetShaderInfoLog(apiHandle, 2048, NULL, infoLog));
				ASSERT_FORMAT(ret, "Error compiling %s shader: %s",
					type == ShaderType::VERTEX ? "vertex" : "pixel",
					infoLog);
			}

			get(handle)->apiHandle = apiHandle;
			get(handle)->type = type;
			get(handle)->timestamp = FileSystem::FileReader::getFileLastWriteTimestamp(filePath);
			handleList[FileSystem::FilePath(filePath)] = handle;
			return handle;
		}

		ResourceHandle<Shader> ShaderResourceManager::getFromFile(const char* filePath)
		{
			FileSystem::FilePath path(filePath);
			ASSERT(handleList.count(path), "Trying to get shader from file that was not compiled yet.");
			return handleList[path];
		}

		void ShaderResourceManager::reloadShaders()
		{
			for (const auto& entry : handleList)
			{
				ResourceHandle<Shader> handle = entry.second;
				uint64_t current_timestamp = FileSystem::FileReader::getFileLastWriteTimestamp(entry.first.path);
				if (current_timestamp > get(handle)->timestamp)
				{
					compileShader(entry.first.path);
				}
			}
		}

		ShaderPipeline ShaderResourceManager::createLinkedShaderPipeline(ResourceHandle<Shader> vs, ResourceHandle<Shader> ps)
		{
			ShaderPipeline shaderPipeline(vs, ps);
			linkShaders(shaderPipeline);
			return shaderPipeline;
		}

		void ShaderResourceManager::linkShaders(ShaderPipeline& shaderPipeline)
		{
			if (shaderPipeline.apiHandle == HANDLE_INVALID)
			{
				GL(shaderPipeline.apiHandle = glCreateProgram());
			}

			uint32_t vsHandle = get(shaderPipeline.vs)->apiHandle;
			uint32_t psHandle = get(shaderPipeline.ps)->apiHandle;

			GL(glAttachShader(shaderPipeline.apiHandle, vsHandle));
			GL(glAttachShader(shaderPipeline.apiHandle, psHandle));
			GL(glLinkProgram(shaderPipeline.apiHandle));
			int ret = 0;
			GL(glGetProgramiv(shaderPipeline.apiHandle, GL_LINK_STATUS, &ret));
			if (!ret)
			{
				char infoLog[2048];
				GL(glGetProgramInfoLog(shaderPipeline.apiHandle, 2048, NULL, infoLog));
				ASSERT_FORMAT(ret, "Error linking shader program: %s",
					infoLog);
			}
		}

		void ShaderResourceManager::bindShaderPipeline(ShaderPipeline& shaderPipeline)
		{
			shaderPipeline.bind();
		}
	}
}
