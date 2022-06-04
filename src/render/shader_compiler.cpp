#include  "render/shader_compiler.h"
#include "render/shader.h"
#include "glad/glad.h"
#include "debugging/gl.h"
#include "debugging/assert.h"
#include "core/hash.h"

std::vector<Shader*> ShaderCompiler::shaderList;

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
		GL(glGetShaderInfoLog(outHandle, 2048, NULL, infoLog));
		ASSERT_FORMAT(ret, "Error linking shader program: %s",
			infoLog);
	}
}

void ShaderCompiler::CompileAndLinkShader(Shader* outShader, const char* VS_src, const char* PS_src)
{
	CompileShader(ShaderType::VERTEX, VS_src, outShader->VS_handle);
	CompileShader(ShaderType::PIXEL, PS_src, outShader->PS_handle);
	outShader->VS_srcHash = HashString(VS_src);
	outShader->PS_srcHash = HashString(PS_src);
	LinkShader(outShader->VS_handle, outShader->PS_handle, outShader->handle);
}

void ShaderCompiler::CheckForDirtyShaderFiles()
{

}

void ShaderCompiler::ReloadDirtyShaders()
{

}
