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

	ResourceHandle<Shader> compileShader(const char* resourcePath);
	void setShaderUniform(ResourceHandle<Shader> shaderHandle, const char* uName, const glm::mat4& uValue);
	void setShaderUniform(ResourceHandle<Shader> shaderHandle, const char* uName, const int& uValue);
	void setShaderUniform(ResourceHandle<Shader> shaderHandle, const char* uName, const float& uValue);
	void setShaderUniform(ResourceHandle<Shader> shaderHandle, const char* uName, const glm::vec3& uValue);
	
	ShaderPipeline createShaderPipeline(ResourceHandle<Shader> vs, ResourceHandle<Shader> ps);
	void bindShaderPipeline(const ShaderPipeline& shaderPipeline);

	// TODO_MEMORY, TODO_TEXTURE: Texture freeing (on demand/destruction)
};

extern ShaderResourceManager g_shaderResourceManager;
