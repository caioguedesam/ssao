#pragma once

#include <stdint.h>
#include <glm/glm.hpp>

class Shader
{
public:
	uint32_t handle = UINT32_MAX;

	uint32_t VS_handle = UINT32_MAX;
	const char* VS_path;

	uint32_t PS_handle = UINT32_MAX;
	const char* PS_path;

	uint64_t timestamp = 0;

	void Bind();
	void SetUniform(const char* uName, const glm::mat4& uValue);
	void SetUniform(const char* uName, const int& uValue);
	void SetUniform(const char* uName, const glm::vec3& uValue);
};
