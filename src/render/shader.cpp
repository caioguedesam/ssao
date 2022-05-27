#include "render/shader.h"

#include <cstring>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "debugging/assert.h"
#include "debugging/gl.h"

void Shader::InitAndCompile(const char* vSrc, const char* fSrc)
{
	if (programHandle != UINT32_MAX)
	{
		GL(glDeleteProgram(programHandle));
	}
	GL(programHandle = glCreateProgram());
	if (!vertexSrc || strcmp(vertexSrc, vSrc) != 0)
	{
		vertexSrc = vSrc;
		if (vertexHandle == UINT32_MAX)
		{
			GL(vertexHandle = glCreateShader(GL_VERTEX_SHADER));
		}
		GL(glShaderSource(vertexHandle, 1, &vertexSrc, NULL));
		GL(glCompileShader(vertexHandle));
		int  success;
		char infoLog[512];
		GL(glGetShaderInfoLog(vertexHandle, 512, NULL, infoLog));
		GL(glGetShaderiv(vertexHandle, GL_COMPILE_STATUS, &success));
		ASSERT(success, "Error compiling vertex shader.");
	}
	if (!fragmentSrc || strcmp(fragmentSrc, fSrc) != 0)
	{
		fragmentSrc = fSrc;
		if (fragmentHandle == UINT32_MAX)
		{
			GL(fragmentHandle = glCreateShader(GL_FRAGMENT_SHADER));
		}
		GL(glShaderSource(fragmentHandle, 1, &fragmentSrc, NULL));
		GL(glCompileShader(fragmentHandle));
		int  success;
		char infoLog[512];
		GL(glGetShaderInfoLog(fragmentHandle, 512, NULL, infoLog));
		GL(glGetShaderiv(fragmentHandle, GL_COMPILE_STATUS, &success));
		ASSERT(success, "Error compiling fragment shader.");
	}
	GL(glAttachShader(programHandle, vertexHandle));
	GL(glAttachShader(programHandle, fragmentHandle));
	GL(glLinkProgram(programHandle));
	int  success;
	char infoLog[512];
	GL(glGetProgramiv(programHandle, GL_LINK_STATUS, &success));
	ASSERT(success, "Error linking shader program.");
}

void Shader::Bind()
{
	GL(glUseProgram(programHandle));
}

void Shader::SetUniform(const char* uName, const glm::mat4& uValue)
{
	GLint location;
	GL(location = glGetUniformLocation(programHandle, uName));	// TODO: Cache this
	GL(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uValue)));
}