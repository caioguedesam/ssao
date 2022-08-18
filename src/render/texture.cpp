#include "stdafx.h"
#include "render/texture.h"

#include <glad/glad.h>
#include "debugging/gl.h"

namespace Ty
{
	namespace Graphics
	{
		void Texture::init(const TextureDesc desc, void* pData)
		{
			if (apiHandle == HANDLE_INVALID)
			{
				GL(glGenTextures(1, &apiHandle));
			}

			this->desc = desc;
			setData(pData);

			bind(0);

			GLenum glInternalFormat = GLInternalFormat(desc.format);
			GLenum glFormat, glType;
			GLFormatAndType(desc.format, glFormat, glType);

			GL(glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, desc.width, desc.height, 0, glFormat, glType, pData));
			GL(glGenerateMipmap(GL_TEXTURE_2D));	// TODO_#MIPS: Support proper mip map generation

			GLenum minFilter = GLMinFilter(desc.params);
			GLenum magFilter = GLMagFilter(desc.params);
			GLenum wrapU = GLWrapU(desc.params);
			GLenum wrapV = GLWrapV(desc.params);
			GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapU));
			GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapV));
			GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter));
			GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter));
		}

		void Texture::setData(void* pData)
		{
			ASSERT(apiHandle != HANDLE_INVALID, "Trying to set data to invalid texture resource.");
			this->pData = pData;
		}

		void Texture::bind(uint32_t texUnit)
		{
			ASSERT(apiHandle != HANDLE_INVALID, "Trying to bind invalid texture resource.");
			GL(glActiveTexture(GL_TEXTURE0 + texUnit));
			// TODO_TEXTURE: Support multiple texture dimensions
			GL(glBindTexture(GL_TEXTURE_2D, apiHandle));
		}

		GLenum Texture::GLInternalFormat(TextureFormat texFormat)
		{
			switch (texFormat)
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
				ASSERT_FORMAT(0, "No corresponding OpenGL internal format for texture format %d", static_cast<int>(texFormat));
				return GL_NONE;
			}
		}

		void Texture::GLFormatAndType(TextureFormat texFormat, GLenum& outFormat, GLenum& outType)
		{
			switch (texFormat)
			{
			case TextureFormat::R8_FLOAT:
				outFormat = GL_RED;
				outType = GL_FLOAT;
				break;
			case TextureFormat::R8_G8_B8_UNORM:
				outFormat = GL_RGB;
				outType = GL_UNSIGNED_BYTE;
				break;
			case TextureFormat::R8_G8_B8_A8_UNORM:
				outFormat = GL_RGBA;
				outType = GL_UNSIGNED_BYTE;
				break;
			case TextureFormat::R16_G16_B16_A16_FLOAT:
				outFormat = GL_RGBA;
				outType = GL_FLOAT;
				break;
			case TextureFormat::R32_G32_B32_FLOAT:
				outFormat = GL_RGB;
				outType = GL_FLOAT;
				break;
			default:
				ASSERT_FORMAT(0, "No corresponding OpenGL format/type for texture format %d", static_cast<int>(texFormat));
				outFormat = outType = GL_NONE;
				break;
			}
		}

		GLenum Texture::GLMinFilter(TextureParams params)
		{
			// TODO_#MIPS: Support mip map filtering options
			if (params & TextureParams::MIN_FILTER_NEAREST) return GL_NEAREST;
			if (params & TextureParams::MIN_FILTER_LINEAR) return GL_LINEAR;
			else return GL_NEAREST;	// default
		}

		GLenum Texture::GLMagFilter(TextureParams params)
		{
			if (params & TextureParams::MAG_FILTER_NEAREST) return GL_NEAREST;
			if (params & TextureParams::MAG_FILTER_LINEAR) return GL_LINEAR;
			else return GL_NEAREST;	// default
		}

		GLenum Texture::GLWrapU(TextureParams params)
		{
			if (params & TextureParams::WRAP_REPEAT) return GL_REPEAT;
			if (params & TextureParams::WRAP_MIRRORED_REPEAT) return GL_MIRRORED_REPEAT;
			if (params & TextureParams::WRAP_CLAMP_EDGE) return GL_CLAMP_TO_EDGE;
			if (params & TextureParams::WRAP_CLAMP_BORDER) return GL_CLAMP_TO_BORDER;
			else return GL_REPEAT;	// default
		}

		GLenum Texture::GLWrapV(TextureParams params)
		{
			if (params & TextureParams::WRAP_REPEAT) return GL_REPEAT;
			if (params & TextureParams::WRAP_MIRRORED_REPEAT) return GL_MIRRORED_REPEAT;
			if (params & TextureParams::WRAP_CLAMP_EDGE) return GL_CLAMP_TO_EDGE;
			if (params & TextureParams::WRAP_CLAMP_BORDER) return GL_CLAMP_TO_BORDER;
			else return GL_REPEAT;	// default
		}
	}
}