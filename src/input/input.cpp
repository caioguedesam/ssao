#include "stdafx.h"
#include "input/input.h"

MouseData Input::mouseData;
bool Input::gotMouseData;

void Input::Init()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
	mouseData.position = glm::ivec2(g_screenWidth / 2, g_screenHeight / 2);
}

void Input::UpdateMouseData(SDL_Event& e)
{
	mouseData.offset.x = e.motion.xrel;
	mouseData.offset.y = e.motion.yrel;
	mouseData.position.x += mouseData.offset.x;
	mouseData.position.y += mouseData.offset.y;
	gotMouseData = true;
}

void Input::Update()
{
	if (!gotMouseData)
	{
		mouseData.offset.x = 0;
		mouseData.offset.y = 0;
	}

	gotMouseData = false;
}