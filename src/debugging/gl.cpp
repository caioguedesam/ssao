#include "stdafx.h"
#include "debugging/gl.h"
#include "debugging/assert.h"

#include <glad/glad.h>

#define GL_VERBOSE		0

const char* gl_errors[] =
{
	"GL_INVALID_ENUM",
	"GL_INVALID_VALUE",
	"GL_INVALID_OPERATION",
	"GL_STACK_OVERFLOW",
	"GL_STACK_UNDERFLOW",
	"GL_OUT_OF_MEMORY",
	"GL_INVALID_FRAMEBUFFER_OPERATION",
};

void gl_check_error(const char* statement, const char* fname, int line)
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		uint32_t err_index = err - 0x0500;
		ASSERT_FORMAT(0, "OpenGL error %08x(%s), at %s:%i - for %s\n", err, gl_errors[err_index], fname, line, statement);
	}
#if GL_VERBOSE
	else
	{
		printf("%s\n", statement);
	}
#endif
}