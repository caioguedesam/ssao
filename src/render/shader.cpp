#include "stdafx.h"
#include "render/shader.h"

#include <glad/glad.h>

#include "debugging/assert.h"
#include "debugging/gl.h"

void Shader::setUniform(const char* uName, const glm::mat4& uValue)
{
	GLint location;
	GL(location = glGetUniformLocation(apiHandle, uName));	// TODO: Cache this
	GL(glUniformMatrix4fv(location, 1, GL_FALSE, &uValue[0][0]));
}

void Shader::setUniform(const char* uName, const int& uValue)
{
	GLint location;
	GL(location = glGetUniformLocation(apiHandle, uName));	// TODO: Cache this
	GL(glUniform1i(location, uValue));
}

void Shader::setUniform(const char* uName, const float& uValue)
{
	GLint location;
	GL(location = glGetUniformLocation(apiHandle, uName));	// TODO: Cache this
	GL(glUniform1f(location, uValue));
}

void Shader::setUniform(const char* uName, const glm::vec3& uValue)
{
	GLint location;
	GL(location = glGetUniformLocation(apiHandle, uName));	// TODO: Cache this
	GL(glUniform3fv(location, 1, &uValue[0]));
}

ShaderPipeline::ShaderPipeline(ResourceHandle<Shader> vs, ResourceHandle<Shader> ps)
{
	this->vs = vs;
	this->ps = ps;
}

void ShaderPipeline::bind()
{
	ASSERT(apiHandle != 0, "Trying to bind invalid shader pipeline");
	GL(glUseProgram(apiHandle));
}