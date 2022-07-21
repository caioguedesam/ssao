#pragma once
#include "stdafx.h"
#include "resource/resource_manager.h"
#include "render/shader.h"
#include "core/hash.h"

struct ShaderResourceManager : ResourceManager<Shader>
{
	std::unordered_map<const char*, ResourceHandle<Shader>, HashFunction_CSTR> handleList;

	void init();
	void destroy();

	ResourceHandle<Shader> compileShader(const char* filePath);
	ResourceHandle<Shader> getFromFile(const char* filePath);
	
	ShaderPipeline createLinkedShaderPipeline(ResourceHandle<Shader> vs, ResourceHandle<Shader> ps);
	void linkShaders(ShaderPipeline& shaderPipeline);
	void bindShaderPipeline(ShaderPipeline& shaderPipeline);

	// TODO_MEMORY, TODO_TEXTURE: Texture freeing (on demand/destruction)
};

extern ShaderResourceManager g_shaderResourceManager;
