#include "stdafx.h"
#include "render/shader.h"

#include <glad/glad.h>

#include "core/math.h"
#include "debugging/assert.h"
#include "debugging/gl.h"

namespace Ty
{
	namespace Graphics
	{
		void ShaderPipeline::set_uniform(const char* bind_name, const Math::m4f& bind_value)
		{
			ASSERT(api_handle != HANDLE_INVALID, "Trying to set uniform to invalid shader resource.");
			GLint location;
			GL(location = glGetUniformLocation(api_handle, bind_name));	// TODO: Cache this
			GL(glUniformMatrix4fv(location, 1, GL_FALSE, &bind_value.m00));
		}

		void ShaderPipeline::set_uniform(const char* bind_name, const int& bind_value)
		{
			ASSERT(api_handle != HANDLE_INVALID, "Trying to set uniform to invalid shader resource.");
			GLint location;
			GL(location = glGetUniformLocation(api_handle, bind_name));	// TODO: Cache this
			GL(glUniform1i(location, bind_value));
		}

		void ShaderPipeline::set_uniform(const char* bind_name, const float& bind_value)
		{
			ASSERT(api_handle != HANDLE_INVALID, "Trying to set uniform to invalid shader resource.");
			GLint location;
			GL(location = glGetUniformLocation(api_handle, bind_name));	// TODO: Cache this
			GL(glUniform1f(location, bind_value));
		}

		void ShaderPipeline::set_uniform(const char* bind_name, const Math::v3f& bind_value)
		{
			ASSERT(api_handle != HANDLE_INVALID, "Trying to set uniform to invalid shader resource.");
			GLint location;
			GL(location = glGetUniformLocation(api_handle, bind_name));	// TODO: Cache this
			GL(glUniform3fv(location, 1, &bind_value.x));
		}

		ShaderPipeline::ShaderPipeline() {}

		ShaderPipeline::ShaderPipeline(ResourceHandle<Shader> vs, ResourceHandle<Shader> ps)
		{
			this->vs = vs;
			this->ps = ps;
		}

		void ShaderPipeline::bind()
		{
			ASSERT(api_handle != HANDLE_INVALID, "Trying to bind invalid shader pipeline.");
			GL(glUseProgram(api_handle));
		}
	}
}