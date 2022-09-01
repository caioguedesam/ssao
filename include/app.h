#pragma once

#include "globals.h"
#include "render/renderer.h"

namespace Ty
{
	class App
	{
	public:
		bool is_running;

		Graphics::Renderer renderer;

		void init();
		void destroy();

		void poll_events(double dt);
		void run();
	};
}
