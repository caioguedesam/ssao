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
			uint32_t apiHandle = HANDLE_INVALID;
			ResourceHandle<Texture> targetOutputs[MAX_RENDER_OUTPUTS];
			uint32_t depthBufferApiHandle = HANDLE_INVALID;

			void bind();
			void unbind();

			void init(uint32_t w, uint32_t h);
			void clear(const float& r = 0.0f, const float& g = 0.0f, const float& b = 0.0f, const float& a = 0.0f);
			void setOutput(ResourceHandle<Texture> textureHandle, uint32_t slot);
			void updateOutputs();
			bool isReady();
		};
	}
}
