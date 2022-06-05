#pragma once

#include <set>
#include "stdafx.h"

class Shader;

class ShaderCompiler
{
public:
	enum class ShaderType : uint32_t
	{
		VERTEX,
		PIXEL
	};

	//static std::unordered_map<uint32_t, std::set<Shader*>> shadersWithFile;
	static std::set<Shader*> shaderSet;

	static void CompileShader(ShaderType type, const char* src, uint32_t& outHandle);
	static void LinkShader(uint32_t VS_handle, uint32_t PS_handle, uint32_t& outHandle);
	static void CompileAndLinkShader(Shader* outShader, const char* VS_path, const char* PS_path);

	static std::set<Shader*> CheckForDirtyShaderFiles();
	static void ReloadDirtyShaders();
};
