#include "stdafx.h"
#include "render/texture.h"

#include <glad/glad.h>
#include "debugging/gl.h"

namespace Ty
{
	namespace Graphics
	{
		void Texture::init(const TextureDesc desc, void* data)
		{
			if (api_handle == HANDLE_INVALID)
			{
				GL(glGenTextures(1, &api_handle));
			}

			this->desc = desc;
			set_data(data);

			bind(0);

			GLenum gl_internal_format = gl_get_internal_format(desc.format);
			GLenum gl_format, gl_type;
			gl_get_format_and_type(desc.format, gl_format, gl_type);

			GL(glTexImage2D(GL_TEXTURE_2D, 0, gl_internal_format, desc.width, desc.height, 0, gl_format, gl_type, data));
			GL(glGenerateMipmap(GL_TEXTURE_2D));	// TODO_#MIPS: Support proper mip map generation

			GLenum filter_min = gl_get_filter_min(desc.params);
			GLenum filter_mag = gl_get_filter_mag(desc.params);
			GLenum wrap_u = gl_get_wrap_u(desc.params);
			GLenum wrap_v = gl_get_wrap_v(desc.params);
			GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_u));
			GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_v));
			GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min));
			GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mag));
		}

		void Texture::set_data(void* data)
		{
			ASSERT(api_handle != HANDLE_INVALID, "Trying to set data to invalid texture resource.");
			this->pData = data;
		}

		void Texture::bind(uint32_t tex_slot)
		{
			ASSERT(api_handle != HANDLE_INVALID, "Trying to bind invalid texture resource.");
			GL(glActiveTexture(GL_TEXTURE0 + tex_slot));
			// TODO_TEXTURE: Support multiple texture dimensions
			GL(glBindTexture(GL_TEXTURE_2D, api_handle));
		}

		GLenum Texture::gl_get_internal_format(TextureFormat tex_format)
		{
			switch (tex_format)
			{
			case TextureFormat::R8_FLOAT:
				return GL_RED;
			case TextureFormat::R8_G8_B8_UNORM:
				return GL_RGB;
			case TextureFormat::R8_G8_B8_A8_UNORM:
				return GL_RGBA;
			case TextureFormat::R16_G16_B16_A16_FLOAT:
				return GL_RGBA16F;
			case TextureFormat::R32_G32_B32_FLOAT:
				return GL_RGBA32F;
			default:
				ASSERT_FORMAT(0, "No corresponding OpenGL internal format for texture format %d", static_cast<int>(tex_format));
				return GL_NONE;
			}
		}

		void Texture::gl_get_format_and_type(TextureFormat tex_format, GLenum& out_format, GLenum& out_type)
		{
			switch (tex_format)
			{
			case TextureFormat::R8_FLOAT:
				out_format = GL_RED;
				out_type = GL_FLOAT;
				break;
			case TextureFormat::R8_G8_B8_UNORM:
				out_format = GL_RGB;
				out_type = GL_UNSIGNED_BYTE;
				break;
			case TextureFormat::R8_G8_B8_A8_UNORM:
				out_format = GL_RGBA;
				out_type = GL_UNSIGNED_BYTE;
				break;
			case TextureFormat::R16_G16_B16_A16_FLOAT:
				out_format = GL_RGBA;
				out_type = GL_FLOAT;
				break;
			case TextureFormat::R32_G32_B32_FLOAT:
				out_format = GL_RGB;
				out_type = GL_FLOAT;
				break;
			default:
				ASSERT_FORMAT(0, "No corresponding OpenGL format/type for texture format %d", static_cast<int>(tex_format));
				out_format = out_type = GL_NONE;
				break;
			}
		}

		GLenum Texture::gl_get_filter_min(TextureParams params)
		{
			// TODO_#MIPS: Support mip map filtering options
			if (params & TextureParams::MIN_FILTER_NEAREST) return GL_NEAREST;
			if (params & TextureParams::MIN_FILTER_LINEAR) return GL_LINEAR;
			else return GL_NEAREST;	// default
		}

		GLenum Texture::gl_get_filter_mag(TextureParams params)
		{
			if (params & TextureParams::MAG_FILTER_NEAREST) return GL_NEAREST;
			if (params & TextureParams::MAG_FILTER_LINEAR) return GL_LINEAR;
			else return GL_NEAREST;	// default
		}

		GLenum Texture::gl_get_wrap_u(TextureParams params)
		{
			if (params & TextureParams::WRAP_REPEAT) return GL_REPEAT;
			if (params & TextureParams::WRAP_MIRRORED_REPEAT) return GL_MIRRORED_REPEAT;
			if (params & TextureParams::WRAP_CLAMP_EDGE) return GL_CLAMP_TO_EDGE;
			if (params & TextureParams::WRAP_CLAMP_BORDER) return GL_CLAMP_TO_BORDER;
			else return GL_REPEAT;	// default
		}

		GLenum Texture::gl_get_wrap_v(TextureParams params)
		{
			if (params & TextureParams::WRAP_REPEAT) return GL_REPEAT;
			if (params & TextureParams::WRAP_MIRRORED_REPEAT) return GL_MIRRORED_REPEAT;
			if (params & TextureParams::WRAP_CLAMP_EDGE) return GL_CLAMP_TO_EDGE;
			if (params & TextureParams::WRAP_CLAMP_BORDER) return GL_CLAMP_TO_BORDER;
			else return GL_REPEAT;	// default
		}
	}
}