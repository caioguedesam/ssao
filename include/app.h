#pragma once

#include "globals.h"
#include "render/renderer.h"

class App
{
public:
	bool isRunning;

	Renderer renderer;

	void init();
	void destroy();

	void pollEvents(double dt);
	void run();
};
