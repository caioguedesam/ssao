#include <stdio.h>
#include <glad/glad.h>
#include <SDL.h>
#include <SDL_opengl.h>

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_LoadLibrary(NULL);
	SDL_Window* window = SDL_CreateWindow(
		"test!",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		640, 480, SDL_WINDOW_OPENGL
	);
	SDL_GLContext maincontext = SDL_GL_CreateContext(window);

	if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
		printf("failed to initialize GLAD\n");
		return 0;
	}

	printf("Hello world!");
	while (true)
	{
		printf(".");
	}
	return 0;
}