#pragma once

#include "stdafx.h"
#include "resource/resource_manager.h"

typedef unsigned int GLenum;

namespace Ty
{
	namespace Graphics
	{
		enum TextureFormat : uint32_t
		{
			INVALID_TEXTURE = 0,
			R8_FLOAT,
			R8_UNORM,
			R8_G8_UNORM,
			R8_G8_B8_UNORM,
			R8_G8_B8_A8_UNORM,
			R16_G16_B16_A16_FLOAT,
			R32_G32_B32_FLOAT,
		};

		enum TextureParams : uint32_t
		{
			TEXPARAMS_NONE = 0,
			MIN_FILTER_NEAREST = 1 << 0,
			MIN_FILTER_LINEAR = 1 << 1,
			MAG_FILTER_NEAREST = 1 << 2,
			MAG_FILTER_LINEAR = 1 << 3,
			WRAP_REPEAT = 1 << 4,
			WRAP_MIRRORED_REPEAT = 1 << 5,
			WRAP_CLAMP_EDGE = 1 << 6,
			WRAP_CLAMP_BORDER = 1 << 7
		};

		struct TextureDesc
		{
			uint32_t width = 0;
			uint32_t height = 0;
			TextureFormat format = TextureFormat::INVALID_TEXTURE;
			TextureParams params = TextureParams::TEXPARAMS_NONE;
		};

		// TODO: Free resource
		class Texture
		{
		public:
			TextureDesc desc;
			uint32_t api_handle = HANDLE_INVALID;
			void* pData;

			void init(const TextureDesc desc, void* data);
			void set_data(void* data);
			void generate_mips();
			void bind(uint32_t tex_slot);

			// TODO_TEXTURE: Remove these API specific functions from here
			static GLenum gl_get_internal_format(TextureFormat tex_format);
			static void gl_get_format_and_type(TextureFormat tex_format, GLenum& out_format, GLenum& out_type);
			static GLenum gl_get_filter_min(TextureParams params);
			static GLenum gl_get_filter_mag(TextureParams params);
			static GLenum gl_get_wrap_u(TextureParams params);
			static GLenum gl_get_wrap_v(TextureParams params);
		};

	}
}