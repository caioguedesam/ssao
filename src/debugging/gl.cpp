#include "debugging/gl.h"
#include "stdafx.h"

#include <glad/glad.h>

void gl_check_error(const char* statement, const char* fname, int line)
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		printf("OpenGL error %08x, at %s:%i - for %s\n", err, fname, line, statement);
	}
}