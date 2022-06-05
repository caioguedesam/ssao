#include "stdafx.h"
#include "render/shader.h"

#include <glad/glad.h>

#include "debugging/assert.h"
#include "debugging/gl.h"

void Shader::Bind()
{
	GL(glUseProgram(handle));
}

void Shader::SetUniform(const char* uName, const glm::mat4& uValue)
{
	GLint location;
	GL(location = glGetUniformLocation(handle, uName));	// TODO: Cache this
	GL(glUniformMatrix4fv(location, 1, GL_FALSE, &uValue[0][0]));
}

void Shader::SetUniform(const char* uName, const int& uValue)
{
	GLint location;
	GL(location = glGetUniformLocation(handle, uName));	// TODO: Cache this
	GL(glUniform1i(location, uValue));
}

void Shader::SetUniform(const char* uName, const glm::vec3& uValue)
{
	GLint location;
	GL(location = glGetUniformLocation(handle, uName));	// TODO: Cache this
	GL(glUniform3fv(location, 1, &uValue[0]));
}