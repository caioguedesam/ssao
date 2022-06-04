#pragma once

#include <vector>
#include <stdint.h>

class Shader;

class ShaderCompiler
{
public:
	enum class ShaderType : uint32_t
	{
		VERTEX,
		PIXEL
	};

	static std::vector<Shader*> shaderList;
	
	static void CompileShader(ShaderType type, const char* src, uint32_t& outHandle);
	static void LinkShader(uint32_t VS_handle, uint32_t PS_handle, uint32_t& outHandle);
	static void CompileAndLinkShader(Shader* outShader, const char* VS_src, const char* PS_src);

	static void CheckForDirtyShaderFiles();
	static void ReloadDirtyShaders();
};
