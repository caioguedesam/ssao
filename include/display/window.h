#pragma once
#include "stdafx.h"
#include <SDL.h>

namespace Ty
{
	class Window
	{
	public:
		uint32_t width;
		uint32_t height;
		glm::uvec2 position;

		SDL_Window* handle;

		~Window();

		void init(uint32_t width, uint32_t height, uint32_t x, uint32_t y, const char* title);
		void on_resize(uint32_t new_w, uint32_t new_h);
	};
}