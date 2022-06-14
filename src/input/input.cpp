#include "stdafx.h"
#include "input/input.h"

Input::State Input::currentState;
MouseData Input::mouseData;
bool Input::gotMouseData;

void Input::Init()
{
	Input::ChangeInputMode(Input::State::APP);
	mouseData.position = glm::ivec2(g_screenWidth / 2, g_screenHeight / 2);
}

void Input::UpdateMouseData(SDL_Event& e)
{
	if (currentState == State::APP)
	{
		mouseData.offset.x = e.motion.xrel;
		mouseData.offset.y = e.motion.yrel;
		mouseData.position.x += mouseData.offset.x;
		mouseData.position.y += mouseData.offset.y;
		gotMouseData = true;
	}
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

void Input::ChangeInputMode(Input::State state)
{
	currentState = state;
	int screenWidth = 0, screenHeight = 0;
	switch (state)
	{
	case Input::State::APP:
		SDL_SetRelativeMouseMode(SDL_TRUE);
		break;
	case Input::State::GUI:
		SDL_SetRelativeMouseMode(SDL_FALSE);
#if _WIN32
		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);
		SetCursorPos(screenWidth / 2, screenHeight / 2);
#endif
		break;
	default:
		break;
	}
}