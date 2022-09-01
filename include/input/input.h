#pragma once
#include "stdafx.h"
#include <SDL.h>

#include "globals.h"

namespace Ty
{
	namespace Input
	{
		struct MouseData
		{
			glm::ivec2 position = glm::ivec2(0);
			glm::ivec2 offset = glm::vec2(0);
		};

		class InputManager
		{
		public:
			enum class State : uint32_t
			{
				APP = 0,
				GUI
			};

			static State current_state;
			static MouseData mouse_data;
			static bool got_mouse_data;

			static void init();

			static void update_mouse_data(SDL_Event& e);
			static void update();
			static void change_input_mode(State state);
		};
	}
}
