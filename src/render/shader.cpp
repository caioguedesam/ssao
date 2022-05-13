#include "render/shader.h"

#include <cstring>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "debugging/assert.h"

void Shader::InitAndCompile(const char* vSrc, const char* fSrc)
{
	if (programHandle != UINT32_MAX)
	{
		glDeleteProgram(programHandle);
	}
	programHandle = glCreateProgram();
	if (!vertexSrc || strcmp(vertexSrc, vSrc) != 0)
	{
		vertexSrc = vSrc;
		if (vertexHandle == UINT32_MAX)
		{
			vertexHandle = glCreateShader(GL_VERTEX_SHADER);
		}
		glShaderSource(vertexHandle, 1, &vertexSrc, NULL);
		glCompileShader(vertexHandle);
		int  success;
		char infoLog[512];
		glGetShaderInfoLog(vertexHandle, 512, NULL, infoLog);
		glGetShaderiv(vertexHandle, GL_COMPILE_STATUS, &success);
		ASSERT(success, "Error compiling vertex shader.");
		//printf("%s\n", infoLog);
	}
	if (!fragmentSrc || strcmp(fragmentSrc, fSrc) != 0)
	{
		fragmentSrc = fSrc;
		if (fragmentHandle == UINT32_MAX)
		{
			fragmentHandle = glCreateShader(GL_FRAGMENT_SHADER);
		}
		glShaderSource(fragmentHandle, 1, &fragmentSrc, NULL);
		glCompileShader(fragmentHandle);
		int  success;
		char infoLog[512];
		glGetShaderInfoLog(vertexHandle, 512, NULL, infoLog);
		ASSERT(glGetError() == GL_NO_ERROR, "");
		glGetShaderiv(fragmentHandle, GL_COMPILE_STATUS, &success);
		ASSERT(success, "Error compiling fragment shader.");
	}
	glAttachShader(programHandle, vertexHandle);
	glAttachShader(programHandle, fragmentHandle);
	glLinkProgram(programHandle);
	int  success;
	char infoLog[512];
	glGetProgramiv(programHandle, GL_LINK_STATUS, &success);
	ASSERT(success, "Error linking shader program.");
}

void Shader::Bind()
{
	glUseProgram(programHandle);
	ASSERT(glGetError() == GL_NO_ERROR, "Couldn't bind shader program.");
}

void Shader::SetUniform(const char* uName, const glm::mat4& uValue)
{
	GLint location = glGetUniformLocation(programHandle, uName);	// TODO: Cache this
	ASSERT(glGetError() == GL_NO_ERROR, "Couldn't get uniform location.");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uValue));
}