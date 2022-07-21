#include "stdafx.h"
#include "resource/shader_resource_manager.h"
#include "file/file_reader.h"
#include "debugging/gl.h"
#include "glad/glad.h"

#define SHADER_SOURCE_MAX_SIZE 4096

ShaderResourceManager g_shaderResourceManager;

void ShaderResourceManager::init() {}

void ShaderResourceManager::destroy() {}

ResourceHandle<Shader> ShaderResourceManager::compileShader(const char* resourcePath)
{
	ResourceHandle<Shader> handle;
	if (handleList.count(resourcePath))
	{
		handle = handleList[resourcePath];
	}
	else
	{
		Shader* shader = new Shader();
		handle = add(shader);
	}

	ShaderType type;
	GLenum glType;
	if (strstr(resourcePath, ".vert"))
	{
		type = ShaderType::VERTEX;
		glType = GL_VERTEX_SHADER;
	}
	else if (strstr(resourcePath, ".frag"))
	{
		type = ShaderType::PIXEL;
		glType = GL_FRAGMENT_SHADER;
	}

	uint32_t apiHandle = get(handle)->apiHandle;
	if (apiHandle == UINT32_MAX)
	{
		GL(apiHandle = glCreateShader(glType));
	}

	char shaderSource[SHADER_SOURCE_MAX_SIZE];
	FileReader::ReadFile(resourcePath, shaderSource);
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

	handleList[resourcePath] = handle;
	return handle;
}

void ShaderResourceManager::setShaderUniform(ResourceHandle<Shader> shaderHandle, const char* uName, const glm::mat4& uValue);
void ShaderResourceManager::setShaderUniform(ResourceHandle<Shader> shaderHandle, const char* uName, const int& uValue);
void ShaderResourceManager::setShaderUniform(ResourceHandle<Shader> shaderHandle, const char* uName, const float& uValue);
void ShaderResourceManager::setShaderUniform(ResourceHandle<Shader> shaderHandle, const char* uName, const glm::vec3& uValue);

ShaderPipeline ShaderResourceManager::createShaderPipeline(ResourceHandle<Shader> vs, ResourceHandle<Shader> ps)
{
	ShaderPipeline shaderPipeline(vs, ps);
	return shaderPipeline;
}

void ShaderResourceManager::bindShaderPipeline(const ShaderPipeline& shaderPipeline)
{
	shaderPipeline.bind();
}
