#include "stdafx.h"
#include "display/window.h"

Window::~Window()
{
	if (handle)
	{
		SDL_DestroyWindow(handle);
	}
}

void Window::Init(uint32_t width, uint32_t height, uint32_t x, uint32_t y, const char* title)
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

void Window::OnResize(uint32_t newWidth, uint32_t newHeight)
{
	width = newWidth;
	height = newHeight;
}