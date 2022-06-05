#pragma once
#include "stdafx.h"
#include <SDL.h>

class Window
{
public:
	uint32_t width;
	uint32_t height;
	glm::uvec2 position;

	SDL_Window* handle;

	~Window();

	void Init(uint32_t width, uint32_t height, uint32_t x, uint32_t y, const char* title);
	void OnResize(uint32_t newWidth, uint32_t newHeight);
};
