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
		ShaderResourceManager shader_resource_manager;

		void ShaderResourceManager::init()
		{
			// Fetch and compile all shaders
			std::vector<FileSystem::FilePath> files = FileSystem::FileReader::get_file_names_from_path(SHADERS_PATH);
			for (int i = 0; i < files.size(); i++)
			{
				char path[MAX_PATH] = "";
				strcat(path, SHADERS_PATH);
				strcat(path, files[i].path);
				compile_shader(path);
			}

			// TODO_SHADER: Set up shader hot reload again
		}

		void ShaderResourceManager::destroy() {}

		ResourceHandle<Shader> ShaderResourceManager::compile_shader(const char* file_path)
		{
			ResourceHandle<Shader> handle;
			if (handle_list.count(file_path))
			{
				handle = handle_list[file_path];
			}
			else
			{
				Shader* shader = new Shader();
				handle = add(shader);
			}

			ShaderType type;
			GLenum gl_type;
			if (strstr(file_path, ".vert"))
			{
				type = ShaderType::VERTEX;
				gl_type = GL_VERTEX_SHADER;
			}
			else if (strstr(file_path, ".frag"))
			{
				type = ShaderType::PIXEL;
				gl_type = GL_FRAGMENT_SHADER;
			}

			uint32_t api_handle = get(handle)->api_handle;
			if (api_handle == HANDLE_INVALID)
			{
				GL(api_handle = glCreateShader(gl_type));
			}

			char file_buffer[SHADER_SOURCE_MAX_SIZE];
			FileSystem::FileReader::read_file(file_path, file_buffer);	// TODO_SHADER: Keep going from here: filePath should include resources/shaders/
			const char* source_str = file_buffer;

			GL(glShaderSource(api_handle, 1, &source_str, NULL));
			GL(glCompileShader(api_handle));

			int ret = 0;
			GL(glGetShaderiv(api_handle, GL_COMPILE_STATUS, &ret));
			if (!ret)
			{
				char info_log[2048];
				GL(glGetShaderInfoLog(api_handle, 2048, NULL, info_log));
				ASSERT_FORMAT(ret, "Error compiling %s shader: %s",
					type == ShaderType::VERTEX ? "vertex" : "pixel",
					info_log);
			}

			get(handle)->api_handle = api_handle;
			get(handle)->type = type;
			handle_list[FileSystem::FilePath(file_path)] = handle;
			return handle;
		}

		ResourceHandle<Shader> ShaderResourceManager::get_from_file(const char* file_path)
		{
			FileSystem::FilePath path(file_path);
			ASSERT(handle_list.count(path), "Trying to get shader from file that was not compiled yet.");
			return handle_list[path];
		}

		ShaderPipeline ShaderResourceManager::create_linked_shader_pipeline(ResourceHandle<Shader> vs, ResourceHandle<Shader> ps)
		{
			ShaderPipeline shader_pipeline(vs, ps);
			link_shaders(shader_pipeline);
			return shader_pipeline;
		}

		void ShaderResourceManager::link_shaders(ShaderPipeline& shader_pipeline)
		{
			if (shader_pipeline.api_handle == HANDLE_INVALID)
			{
				GL(shader_pipeline.api_handle = glCreateProgram());
			}

			uint32_t vs_handle = get(shader_pipeline.vs)->api_handle;
			uint32_t ps_handle = get(shader_pipeline.ps)->api_handle;

			GL(glAttachShader(shader_pipeline.api_handle, vs_handle));
			GL(glAttachShader(shader_pipeline.api_handle, ps_handle));
			GL(glLinkProgram(shader_pipeline.api_handle));
			int ret = 0;
			GL(glGetProgramiv(shader_pipeline.api_handle, GL_LINK_STATUS, &ret));
			if (!ret)
			{
				char info_log[2048];
				GL(glGetProgramInfoLog(shader_pipeline.api_handle, 2048, NULL, info_log));
				ASSERT_FORMAT(ret, "Error linking shader program: %s",
					info_log);
			}
		}

		void ShaderResourceManager::bind_shader_pipeline(ShaderPipeline& shaderPipeline)
		{
			shaderPipeline.bind();
		}

	}
}
