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

void DestroySDL()
{
	SDL_Quit();
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
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
		);
		m_width = width;
		m_height = height;
		m_position = glm::uvec2(x, y);
	}

	void OnResize(uint32_t newWidth, uint32_t newHeight)
	{
		m_width = newWidth;
		m_height = newHeight;
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

	void SetViewport(uint32_t width, uint32_t height, uint32_t x, uint32_t y)
	{
		glViewport(x, y, width, height);
	}

	void Init(uint32_t windowWidth, uint32_t windowHeight, uint32_t windowX, uint32_t windowY, const char* windowTitle)
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		CreateNewWindow(windowWidth, windowHeight, windowX, windowY, windowTitle);
		CreateNewRenderContext();
		RetrieveAPIFunctionLocations();

		glEnable(GL_DEPTH_TEST);

		SetViewport(windowWidth, windowHeight, windowX, windowY);
	}

	void OnResize(uint32_t newWidth, uint32_t newHeight)
	{
		m_window->OnResize(newWidth, newHeight);
		SetViewport(newWidth, newHeight, m_window->m_position.x, m_window->m_position.y);
	}

	void Render()
	{
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SDL_GL_SwapWindow(m_window->m_handle);
	}
};

class App
{
public:
	bool m_isRunning;

	Renderer m_renderer;

	void Init()
	{
		m_renderer.Init(640, 480, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, "SSAO");
		m_isRunning = true;
	}

	void PollEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{
				SDL_Keysym key = event.key.keysym;
				switch (key.sym)
				{
				case SDLK_ESCAPE:
				{
					m_isRunning = false;
				} break;
				}
			}
			else if (event.type == SDL_WINDOWEVENT)
			{
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					uint32_t newWidth = event.window.data1;
					uint32_t newHeight = event.window.data2;
					m_renderer.OnResize(newWidth, newHeight);
				}
			}
			else if (event.type == SDL_QUIT)
			{
				m_isRunning = false;
			}
		}
	}

	void Run()
	{
		while (m_isRunning)
		{
			PollEvents();
			// TODO: update logic here
			m_renderer.Render();
		}
	}
};

int main(int argc, char* argv[])
{
	InitSDL();
	App app;
	app.Init();

	app.Run();

	DestroySDL();
	return 0;
}