#include "stdafx.h"
#include <SDL.h>

#include "app.h"

void InitSDL()
{
	uint32_t initFlags =
		SDL_INIT_VIDEO
		| SDL_INIT_TIMER
		| SDL_INIT_JOYSTICK;

	int result = SDL_Init(initFlags);
	ASSERT_ZERO(result, "Failed to initialize SDL2.");
	result = SDL_GL_LoadLibrary(NULL);
	ASSERT_ZERO(result, "Failed to load SDL2 OpenGL library.");
}

void DestroySDL()
{
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	InitSDL();
	Ty::App app;
	app.init();

	app.run();

	app.destroy();
	DestroySDL();
	return 0;
}