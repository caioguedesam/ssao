#include "stdafx.h"
#include "input/input.h"

namespace Ty
{
	namespace Input
	{
		InputManager::State InputManager::currentState;
		MouseData InputManager::mouseData;
		bool InputManager::gotMouseData;

		void InputManager::init()
		{
			InputManager::changeInputMode(InputManager::State::APP);
			mouseData.position = glm::ivec2(g_screenWidth / 2, g_screenHeight / 2);
		}

		void InputManager::updateMouseData(SDL_Event& e)
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

		void InputManager::update()
		{
			if (!gotMouseData)
			{
				mouseData.offset.x = 0;
				mouseData.offset.y = 0;
			}

			gotMouseData = false;
		}

		void InputManager::changeInputMode(InputManager::State state)
		{
			currentState = state;
			int screenWidth = 0, screenHeight = 0;
			switch (state)
			{
			case InputManager::State::APP:
				SDL_SetRelativeMouseMode(SDL_TRUE);
				break;
			case InputManager::State::GUI:
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
	}
}