#pragma once
#include "stdafx.h"
#include <SDL.h>

#include "globals.h"

struct MouseData
{
	glm::ivec2 position = glm::ivec2(0);
	glm::ivec2 offset = glm::vec2(0);
};

class Input
{
public:
	static MouseData mouseData;
	static bool gotMouseData;

	static void Init();

	static void UpdateMouseData(SDL_Event& e);
	static void Update();
};
