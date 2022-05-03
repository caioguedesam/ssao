#include <stdio.h>
#include <stdint.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <SDL.h>

#if _DEBUG
#include <cassert>
#define ASSERT(EXPR, MSG) do {assert(EXPR && MSG);} while(false)
#define ASSERT_ZERO(EXPR, MSG) do {assert(!EXPR && MSG);} while(false)
#else
#define ASSERT(EXPR, MSG) do {} while(false)
#define ASSERT_ZERO(EXPR, MSG) do {} while(false)
#endif

void InitSDL()
{
	uint32_t initFlags =
		SDL_INIT_VIDEO
		| SDL_INIT_TIMER;

	int result = SDL_Init(initFlags);
	ASSERT_ZERO(result, "Failed to initialize SDL2.");
	result = SDL_GL_LoadLibrary(NULL);
	ASSERT_ZERO(result, "Failed to load SDL2 OpenGL library.");
}

class Window
{
public:
	uint32_t m_width;
	uint32_t m_height;
	glm::uvec2 m_position;

	SDL_Window* m_handle;

	~Window()
	{
		if (m_handle)
		{
			SDL_DestroyWindow(m_handle);
		}
	}

	void Init(uint32_t width, uint32_t height, uint32_t x, uint32_t y, const char* title)
	{
		m_handle = SDL_CreateWindow(
			title,
			x, y,
			width, height, 
			SDL_WINDOW_OPENGL
		);
		m_width = width;
		m_height = height;
		m_position = glm::uvec2(x, y);
	}
};

class Renderer
{
public:
	Window* m_window;
	SDL_GLContext m_glContextHandle;

	~Renderer()
	{
		if (m_glContextHandle)
		{
			SDL_GL_DeleteContext(m_glContextHandle);
		}
		if (m_window) delete m_window;
	}

	void CreateNewWindow(uint32_t width, uint32_t height, uint32_t x, uint32_t y, const char* title)
	{
		m_window = new Window();
		m_window->Init(width, height, x, y, title);
	}

	void CreateNewRenderContext()
	{
		ASSERT(!m_glContextHandle, "Trying to create new OpenGL context when renderer already has one.");
		ASSERT(m_window, "Trying to create new OpenGL context without window associated to renderer.");
		m_glContextHandle = SDL_GL_CreateContext(m_window->m_handle);
		ASSERT(m_glContextHandle, "Failed to create new OpenGL context.");
	}

	void RetrieveAPIFunctionLocations()
	{
		bool result = gladLoadGLLoader(SDL_GL_GetProcAddress);
		ASSERT(result, "Failed to retrieve OpenGL API function locations using GLAD.");
	}

	void Init(uint32_t windowWidth, uint32_t windowHeight, uint32_t windowX, uint32_t windowY, const char* windowTitle)
	{
		CreateNewWindow(windowWidth, windowHeight, windowX, windowY, windowTitle);
		CreateNewRenderContext();
		RetrieveAPIFunctionLocations();
	}
};

int main(int argc, char* argv[])
{
	InitSDL();
	Renderer renderer;
	renderer.Init(640, 480, 0, 0, "SSAO");

	printf("Hello world!");
	return 0;
}