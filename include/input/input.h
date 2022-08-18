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

			static State currentState;
			static MouseData mouseData;
			static bool gotMouseData;

			static void init();

			static void updateMouseData(SDL_Event& e);
			static void update();
			static void changeInputMode(State state);
		};
	}
}
