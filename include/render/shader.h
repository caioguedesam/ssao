#pragma once

#include "stdafx.h"
#include "resource/resource_manager.h"

//class Shader
//{
//public:
//	uint32_t handle = UINT32_MAX;
//
//	uint32_t VS_handle = UINT32_MAX;
//	const char* VS_path;
//
//	uint32_t PS_handle = UINT32_MAX;
//	const char* PS_path;
//
//	uint64_t timestamp = 0;
//
//	void Bind();
//	void SetUniform(const char* uName, const glm::mat4& uValue);
//	void SetUniform(const char* uName, const int& uValue);
//	void SetUniform(const char* uName, const float& uValue);
//	void SetUniform(const char* uName, const glm::vec3& uValue);
//};

enum ShaderType : uint32_t
{
	INVALID = 0,
	VERTEX,
	PIXEL
};

struct Shader
{
	uint32_t apiHandle = UINT32_MAX;
	ShaderType type;
	size_t timestamp = 0;

	void bind();
	void setUniform(const char* uName, const glm::mat4& uValue);
	void setUniform(const char* uName, const int& uValue);
	void setUniform(const char* uName, const float& uValue);
	void setUniform(const char* uName, const glm::vec3& uValue);
};

struct ShaderPipeline
{
	uint32_t apiHandle = 0;

	ResourceHandle<Shader> vs;
	ResourceHandle<Shader> ps;
	// TODO_SHADER: Other shader stages? (geometry, hull...)

	ShaderPipeline(ResourceHandle<Shader> vs, ResourceHandle<Shader> ps);

	void bind();
};