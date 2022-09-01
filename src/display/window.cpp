#include "stdafx.h"
#include "display/window.h"

namespace Ty
{
	Window::~Window()
	{
		if (handle)
		{
			SDL_DestroyWindow(handle);
		}
	}

	void Window::init(uint32_t width, uint32_t height, uint32_t x, uint32_t y, const char* title)
	{
		handle = SDL_CreateWindow(
			title,
			x, y,
			width, height,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
		);
		width = width;
		height = height;
		position = glm::uvec2(x, y);
		SDL_WarpMouseInWindow(handle, 0, 0);
	}

	void Window::on_resize(uint32_t new_w, uint32_t new_h)
	{
		width = new_w;
		height = new_h;
	}
}