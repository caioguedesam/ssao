#include  "render/shader_compiler.h"

#include <sys/stat.h>

#include "render/shader.h"
#include "glad/glad.h"
#include "debugging/gl.h"
#include "debugging/assert.h"
#include "file/file_reader.h"
#include "globals.h"

#if _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

#define SHADER_SOURCE_MAX_SIZE 4096

//std::unordered_map<uint32_t, std::set<Shader*>> ShaderCompiler::shadersWithFile;
std::set<Shader*> ShaderCompiler::shaderSet;

void ShaderCompiler::CompileShader(ShaderType type, const char* src, uint32_t& outHandle)
{
	GLenum glType = type == ShaderType::VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
	if (outHandle == UINT32_MAX)
	{
		GL(outHandle = glCreateShader(glType));
	}
	GL(glShaderSource(outHandle, 1, &src, NULL));
	GL(glCompileShader(outHandle));
	int ret = 0;
	GL(glGetShaderiv(outHandle, GL_COMPILE_STATUS, &ret));
	if (!ret)
	{
		char infoLog[2048];
		GL(glGetShaderInfoLog(outHandle, 2048, NULL, infoLog));
		ASSERT_FORMAT(ret, "Error compiling %s shader: %s",
			glType == GL_VERTEX_SHADER ? "vertex" : "pixel",
			infoLog);
	}
}

void ShaderCompiler::LinkShader(uint32_t VS_handle, uint32_t PS_handle, uint32_t& outHandle)
{
	if (outHandle == UINT32_MAX)
	{
		GL(outHandle = glCreateProgram());
	}
	GL(glAttachShader(outHandle, VS_handle));
	GL(glAttachShader(outHandle, PS_handle));
	GL(glLinkProgram(outHandle));
	int ret = 0;
	GL(glGetProgramiv(outHandle, GL_LINK_STATUS, &ret));
	if (!ret)
	{
		char infoLog[2048];
		GL(glGetProgramInfoLog(outHandle, 2048, NULL, infoLog));
		ASSERT_FORMAT(ret, "Error linking shader program: %s",
			infoLog);
	}
}

void ShaderCompiler::CompileAndLinkShader(Shader* outShader, const char* VS_path, const char* PS_path)
{
	char VS_src[SHADER_SOURCE_MAX_SIZE];
	char PS_src[SHADER_SOURCE_MAX_SIZE];

	FileReader::ReadFile(VS_path, VS_src);
	FileReader::ReadFile(PS_path, PS_src);

	CompileShader(ShaderType::VERTEX, VS_src, outShader->VS_handle);
	CompileShader(ShaderType::PIXEL, PS_src, outShader->PS_handle);
	LinkShader(outShader->VS_handle, outShader->PS_handle, outShader->handle);
	outShader->VS_path = VS_path;
	outShader->PS_path = PS_path;

	struct _stat statBuffer;
	uint64_t VS_timestamp = 0, PS_timestamp = 0;
	int ret = _stat(outShader->VS_path, &statBuffer);
	VS_timestamp = statBuffer.st_mtime;
	ret = _stat(outShader->PS_path, &statBuffer);
	PS_timestamp = statBuffer.st_mtime;
	outShader->timestamp = max(VS_timestamp, PS_timestamp);

	shaderSet.insert(outShader);
}

std::set<Shader*> ShaderCompiler::CheckForDirtyShaderFiles()
{
	std::set<Shader*> result;
	struct _stat statBuffer;
	int ret;
	for (auto& shader : shaderSet)
	{
		ret = _stat(shader->VS_path, &statBuffer);
		ASSERT_ZERO(ret, "Error checking vertex shader at path %s for reloading: %d", shader->VS_path, ret);
		if (statBuffer.st_mtime > shader->timestamp)
		{
			shader->timestamp = statBuffer.st_mtime;
			result.insert(shader);
			continue;
		}
		ret = _stat(shader->PS_path, &statBuffer);
		ASSERT_ZERO(ret, "Error checking pixel shader at path %s for reloading: %d", shader->PS_path, ret);
		if (statBuffer.st_mtime > shader->timestamp)
		{
			shader->timestamp = statBuffer.st_mtime;
			result.insert(shader);
		}
	}

	return result;
}

void ShaderCompiler::ReloadDirtyShaders()
{
	std::set<Shader*> dirtyShaders = CheckForDirtyShaderFiles();
	if (dirtyShaders.size())
	{
		Sleep(100);
	}
	for (auto& shader : dirtyShaders)
	{
		CompileAndLinkShader(shader, shader->VS_path, shader->PS_path);
	}
}
