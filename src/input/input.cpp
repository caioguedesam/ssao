#include "stdafx.h"
#include "input/input.h"

namespace Ty
{
	namespace Input
	{
		InputManager::State InputManager::current_state;
		MouseData InputManager::mouse_data;
		bool InputManager::got_mouse_data;

		void InputManager::init()
		{
			InputManager::change_input_mode(InputManager::State::APP);
			mouse_data.position = { (i32)screen_w / 2, (i32)screen_h / 2 };
		}

		void InputManager::update_mouse_data(SDL_Event& e)
		{
			if (current_state == State::APP)
			{
				mouse_data.offset.x = e.motion.xrel;
				mouse_data.offset.y = e.motion.yrel;
				mouse_data.position.x += mouse_data.offset.x;
				mouse_data.position.y += mouse_data.offset.y;
				got_mouse_data = true;
			}
		}

		void InputManager::update()
		{
			if (!got_mouse_data)
			{
				mouse_data.offset.x = 0;
				mouse_data.offset.y = 0;
			}

			got_mouse_data = false;
		}

		void InputManager::change_input_mode(InputManager::State state)
		{
			current_state = state;
			int w = 0, h = 0;
			switch (state)
			{
			case InputManager::State::APP:
				SDL_SetRelativeMouseMode(SDL_TRUE);
				break;
			case InputManager::State::GUI:
				SDL_SetRelativeMouseMode(SDL_FALSE);
#if _WIN32
				w = GetSystemMetrics(SM_CXSCREEN);
				h = GetSystemMetrics(SM_CYSCREEN);
				SetCursorPos(w / 2, h / 2);
#endif
				break;
			default:
				break;
			}
		}
	}
}