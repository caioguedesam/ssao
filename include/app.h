#pragma once

#include "globals.h"
#include "render/renderer.h"

namespace Ty
{
	class App
	{
	public:
		bool isRunning;

		Graphics::Renderer renderer;

		void init();
		void destroy();

		void pollEvents(double dt);
		void run();
	};
}
