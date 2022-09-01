#include "stdafx.h"
#include <SDL.h>

#include "app.h"

void sdl_init()
{
	uint32_t init_flags =
		SDL_INIT_VIDEO
		| SDL_INIT_TIMER
		| SDL_INIT_JOYSTICK;

	int result = SDL_Init(init_flags);
	ASSERT_ZERO(result, "Failed to initialize SDL2.");
	result = SDL_GL_LoadLibrary(NULL);
	ASSERT_ZERO(result, "Failed to load SDL2 OpenGL library.");
}

void sdl_destroy()
{
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	sdl_init();
	Ty::App app;
	app.init();

	app.run();

	app.destroy();
	sdl_destroy();
	return 0;
}