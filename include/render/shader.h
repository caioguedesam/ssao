#pragma once

#include <stdint.h>
#include <glm/glm.hpp>

class Shader
{
public:
	uint32_t programHandle = UINT32_MAX;

	uint32_t vertexHandle = UINT32_MAX;
	uint32_t fragmentHandle = UINT32_MAX;

	const char* vertexSrc = nullptr;
	const char* fragmentSrc = nullptr;

	void InitAndCompile(const char* vSrc, const char* fSrc);
	void Bind();
	void SetUniform(const char* uName, const glm::mat4& uValue);
	void SetUniform(const char* uName, const int& uValue);
};
