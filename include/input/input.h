#pragma once
#include "stdafx.h"
#include "core/math.h"
#include <SDL.h>

#include "globals.h"

namespace Ty
{
	namespace Input
	{
		struct MouseData
		{
			Math::v2i position;
			Math::v2i offset;
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
