#pragma once

#include "globals.h"
#include "render/renderer.h"

class App
{
public:
	bool isRunning;

	Renderer renderer;

	void Init();

	void PollEvents(double dt);

	void Run();
};
