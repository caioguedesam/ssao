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
			uint32_t apiHandle = HANDLE_INVALID;
			ShaderType type;
			uint64_t timestamp = 0;
		};

		struct ShaderPipeline
		{
			uint32_t apiHandle = HANDLE_INVALID;

			ResourceHandle<Shader> vs;
			ResourceHandle<Shader> ps;
			// TODO_SHADER: Other shader stages? (geometry, hull...)

			ShaderPipeline();
			ShaderPipeline(ResourceHandle<Shader> vs, ResourceHandle<Shader> ps);

			void bind();
			void setUniform(const char* uName, const glm::mat4& uValue);
			void setUniform(const char* uName, const int& uValue);
			void setUniform(const char* uName, const float& uValue);
			void setUniform(const char* uName, const glm::vec3& uValue);
		};
	}
}