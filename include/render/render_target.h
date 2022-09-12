#pragma once

#include "stdafx.h"
#include "resource/texture_resource_manager.h"

#define MAX_RENDER_OUTPUTS 8

namespace Ty
{
	namespace Graphics
	{
		struct RenderTarget
		{
			uint32_t api_handle = HANDLE_INVALID;
			ResourceHandle<Texture> target_outputs[MAX_RENDER_OUTPUTS];
			uint32_t depth_buffer_api_handle = HANDLE_INVALID;

			void bind();
			void unbind();

			void init(uint32_t w, uint32_t h);
			void clear(const float& r = 0.0f, const float& g = 0.0f, const float& b = 0.0f, const float& a = 0.0f);
			void set_output(ResourceHandle<Texture> texture_handle, uint32_t slot);
			void update_outputs();
			void update_output_mips();
			bool is_ready();
		};
	}
}
