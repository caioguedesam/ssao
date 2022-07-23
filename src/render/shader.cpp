#include "stdafx.h"
#include "render/shader.h"

#include <glad/glad.h>

#include "debugging/assert.h"
#include "debugging/gl.h"

void ShaderPipeline::setUniform(const char* uName, const glm::mat4& uValue)
{
	ASSERT(apiHandle != HANDLE_INVALID, "Trying to set uniform to invalid shader resource.");
	GLint location;
	GL(location = glGetUniformLocation(apiHandle, uName));	// TODO: Cache this
	GL(glUniformMatrix4fv(location, 1, GL_FALSE, &uValue[0][0]));
}

void ShaderPipeline::setUniform(const char* uName, const int& uValue)
{
	ASSERT(apiHandle != HANDLE_INVALID, "Trying to set uniform to invalid shader resource.");
	GLint location;
	GL(location = glGetUniformLocation(apiHandle, uName));	// TODO: Cache this
	GL(glUniform1i(location, uValue));
}

void ShaderPipeline::setUniform(const char* uName, const float& uValue)
{
	ASSERT(apiHandle != HANDLE_INVALID, "Trying to set uniform to invalid shader resource.");
	GLint location;
	GL(location = glGetUniformLocation(apiHandle, uName));	// TODO: Cache this
	GL(glUniform1f(location, uValue));
}

void ShaderPipeline::setUniform(const char* uName, const glm::vec3& uValue)
{
	ASSERT(apiHandle != HANDLE_INVALID, "Trying to set uniform to invalid shader resource.");
	GLint location;
	GL(location = glGetUniformLocation(apiHandle, uName));	// TODO: Cache this
	GL(glUniform3fv(location, 1, &uValue[0]));
}

ShaderPipeline::ShaderPipeline() {}

ShaderPipeline::ShaderPipeline(ResourceHandle<Shader> vs, ResourceHandle<Shader> ps)
{
	this->vs = vs;
	this->ps = ps;
}

void ShaderPipeline::bind()
{
	ASSERT(apiHandle != HANDLE_INVALID, "Trying to bind invalid shader pipeline.");
	GL(glUseProgram(apiHandle));
}