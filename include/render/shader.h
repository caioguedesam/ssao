#pragma once

#include "stdafx.h"
#include "resource/resource_manager.h"

namespace Ty
{
	namespace Graphics
	{
		enum ShaderType : uint32_t
		{
			INVALID_SHADER = 0,
			VERTEX,
			PIXEL
		};

		struct Shader
		{
			uint32_t api_handle = HANDLE_INVALID;
			ShaderType type;
			size_t timestamp = 0;
		};

		struct ShaderPipeline
		{
			uint32_t api_handle = HANDLE_INVALID;

			ResourceHandle<Shader> vs;
			ResourceHandle<Shader> ps;
			// TODO_SHADER: Other shader stages? (geometry, hull...)

			ShaderPipeline();
			ShaderPipeline(ResourceHandle<Shader> vs, ResourceHandle<Shader> ps);

			void bind();
			void set_uniform(const char* bind_name, const glm::mat4& bind_value);
			void set_uniform(const char* bind_name, const int& bind_value);
			void set_uniform(const char* bind_name, const float& bind_value);
			void set_uniform(const char* bind_name, const glm::vec3& bind_value);
		};
	}
}