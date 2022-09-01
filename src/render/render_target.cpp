#include "stdafx.h"
#include "render/render_target.h"
#include <glad/glad.h>
#include "debugging/gl.h"

namespace Ty
{
	namespace Graphics
	{
		void RenderTarget::bind()
		{
			GL(glBindFramebuffer(GL_FRAMEBUFFER, api_handle));
		}

		void RenderTarget::unbind()
		{
			GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		}

		void RenderTarget::init(uint32_t w, uint32_t h)
		{
			if (api_handle == HANDLE_INVALID)
			{
				GL(glGenFramebuffers(1, &api_handle));
			}

			bind();

			// Create depth buffer
			GL(glGenRenderbuffers(1, &depth_buffer_api_handle));
			GL(glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer_api_handle));
			GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h));
			GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer_api_handle));

			//unbind();
		}

		void RenderTarget::clear(const float& r, const float& g, const float& b, const float& a)
		{
			bind();
			GL(glClearColor(r, g, b, a));
			GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
			//unbind();
		}

		void RenderTarget::set_output(ResourceHandle<Texture> texture_handle, uint32_t slot)
		{
			ASSERT(texture_handle.is_valid(), "Trying to set invalid texture to render target output.");
			ASSERT(slot < MAX_RENDER_OUTPUTS, "Trying to set texture to output over maximum render target outputs.");

			bind();
			target_outputs[slot] = texture_handle;
			texture_resource_manager.bind_texture(texture_handle, slot);
			GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, GL_TEXTURE_2D, texture_resource_manager.get(texture_handle)->api_handle, 0));
			update_outputs();
			//unbind();
		}

		void RenderTarget::update_outputs()
		{
			static GLenum color_buffers[MAX_RENDER_OUTPUTS];
			memset(color_buffers, 0, sizeof(color_buffers));
			int count = 0;
			for (int i = 0; i < MAX_RENDER_OUTPUTS; i++)
			{
				if (target_outputs[i].is_valid())
				{
					color_buffers[count] = GL_COLOR_ATTACHMENT0 + i;
					count++;
				}
			}
			GL(glDrawBuffers(count, color_buffers));
		}

		bool RenderTarget::is_ready()
		{
			bind();
			int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			//unbind();
			return status == GL_FRAMEBUFFER_COMPLETE;
		}
	}
}