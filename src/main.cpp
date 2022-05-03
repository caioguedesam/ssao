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

// Default resources
// Vertices for 1x1 quad centered on origin
float g_quadVertices[] =
{
	-0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	 0.5f, 0.5f, 0.0f,
	 0.5f,  -0.5f, 0.0f
};
// Indices for 1x1 quad
uint32_t g_quadIndices[] =
{
	0, 2, 1,
	0, 3, 2
};

class Buffer
{
	uint32_t handle;
	uint32_t sizeInBytes;
	uint32_t count;
	void* pData;

	void Set(GLenum bindTarget, uint32_t bufferSize, uint32_t bufferCount, void* bufferData)
	{
		sizeInBytes = bufferSize;
		count = bufferCount;
		pData = bufferData;
		glBindBuffer(bindTarget, handle);
		glBufferData(bindTarget, sizeInBytes, pData, GL_STATIC_DRAW);
	}

	void Init()
	{
		glGenBuffers(1, &handle);
	}
};

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
	uint32_t width;
	uint32_t height;
	glm::uvec2 position;

	SDL_Window* handle;

	~Window()
	{
		if (handle)
		{
			SDL_DestroyWindow(handle);
		}
	}

	void Init(uint32_t width, uint32_t height, uint32_t x, uint32_t y, const char* title)
	{
		handle = SDL_CreateWindow(
			title,
			x, y,
			width, height, 
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
		);
		width = width;
		height = height;
		position = glm::uvec2(x, y);
	}

	void OnResize(uint32_t newWidth, uint32_t newHeight)
	{
		width = newWidth;
		height = newHeight;
	}
};

class Renderer
{
public:
	Window* pWindow;
	SDL_GLContext pGlContextHandle;

	~Renderer()
	{
		if (pGlContextHandle)
		{
			SDL_GL_DeleteContext(pGlContextHandle);
		}
		if (pWindow) delete pWindow;
	}

	void CreateNewWindow(uint32_t width, uint32_t height, uint32_t x, uint32_t y, const char* title)
	{
		pWindow = new Window();
		pWindow->Init(width, height, x, y, title);
	}

	void CreateNewRenderContext()
	{
		ASSERT(!pGlContextHandle, "Trying to create new OpenGL context when renderer already has one.");
		ASSERT(pWindow, "Trying to create new OpenGL context without window associated to renderer.");
		pGlContextHandle = SDL_GL_CreateContext(pWindow->handle);
		ASSERT(pGlContextHandle, "Failed to create new OpenGL context.");
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
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		SetViewport(windowWidth, windowHeight, windowX, windowY);
	}

	void OnResize(uint32_t newWidth, uint32_t newHeight)
	{
		pWindow->OnResize(newWidth, newHeight);
		SetViewport(newWidth, newHeight, pWindow->position.x, pWindow->position.y);
	}

	void Render()
	{
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SDL_GL_SwapWindow(pWindow->handle);
	}
};

class App
{
public:
	bool isRunning;

	Renderer renderer;

	void Init()
	{
		renderer.Init(640, 480, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, "SSAO");
		isRunning = true;
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
					isRunning = false;
				} break;
				}
			}
			else if (event.type == SDL_WINDOWEVENT)
			{
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					uint32_t newWidth = event.window.data1;
					uint32_t newHeight = event.window.data2;
					renderer.OnResize(newWidth, newHeight);
				}
			}
			else if (event.type == SDL_QUIT)
			{
				isRunning = false;
			}
		}
	}

	void Run()
	{
		while (isRunning)
		{
			PollEvents();
			// TODO: update logic here
			renderer.Render();
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