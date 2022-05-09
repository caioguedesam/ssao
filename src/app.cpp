#include "app.h"

#include <SDL.h>

#include "time/time.h"
#include "model/mesh.h"
#include "file/file_reader.h"

void GetDisplayDimensions(uint32_t& w, uint32_t& h)
{
	ASSERT(SDL_WasInit(SDL_INIT_VIDEO), "Trying to get display dimensions without initializing SDL.");
	SDL_DisplayMode DM;
	SDL_GetDesktopDisplayMode(0, &DM);
	w = DM.w;
	h = DM.h;
}

void App::Init()
{
	uint32_t appWidth = APP_DEFAULT_WIDTH;
	uint32_t appHeight = APP_DEFAULT_HEIGHT;
	GetDisplayDimensions(g_screenWidth, g_screenHeight);

	renderer.Init(appWidth, appHeight, (g_screenWidth - appWidth) / 2, (g_screenHeight - appHeight) / 2, "SSAO",
		0, 0, 3.f, 45.f, static_cast<float>(appWidth) / static_cast<float>(appHeight));
	Time::Init();
	Input::Init();

	isRunning = true;
}

void App::PollEvents(double dt)
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
			case SDLK_w:
			{
				renderer.camera.moveAmounts.y = 1;
			} break;
			case SDLK_s:
			{
				renderer.camera.moveAmounts.y = -1;
			} break;
			case SDLK_a:
			{
				renderer.camera.moveAmounts.x = -1;
			} break;
			case SDLK_d:
			{
				renderer.camera.moveAmounts.x = 1;
			} break;
			case SDLK_UP:
			{
				renderer.camera.rotateAmounts.x = 1;
			} break;
			case SDLK_DOWN:
			{
				renderer.camera.rotateAmounts.x = -1;
			} break;
			case SDLK_LEFT:
			{
				renderer.camera.rotateAmounts.y = -1;
			} break;
			case SDLK_RIGHT:
			{
				renderer.camera.rotateAmounts.y = 1;
			} break;
			}
		}
		else if (event.type == SDL_KEYUP)
		{
			SDL_Keysym key = event.key.keysym;
			switch (key.sym)
			{
			case SDLK_w:
			case SDLK_s:
			{
				renderer.camera.moveAmounts.y = 0;
			} break;
			case SDLK_a:
			case SDLK_d:
			{
				renderer.camera.moveAmounts.x = 0;
			} break;
			case SDLK_UP:
			case SDLK_DOWN:
			{
				renderer.camera.rotateAmounts.x = 0;
			} break;
			case SDLK_LEFT:
			case SDLK_RIGHT:
			{
				renderer.camera.rotateAmounts.y = 0;
			} break;
			}
		}
		else if (event.type == SDL_MOUSEMOTION)
		{
			Input::UpdateMouseData(event);
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

void App::Run()
{
	Mesh backpackMesh;
	backpackMesh.LoadMesh(MODELS_PATH"backpack.obj");

	Buffer vb;
	//vb.Init(GL_ARRAY_BUFFER, sizeof(g_quadVertices), sizeof(g_quadVertices) / sizeof(float), g_quadVertices);
	vb.Init(GL_ARRAY_BUFFER, sizeof(float) * backpackMesh.vertices.size(), backpackMesh.vertices.size(), backpackMesh.vertices.data());
	Buffer ib;
	//ib.Init(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_quadIndices), sizeof(g_quadIndices) / sizeof(uint32_t), g_quadIndices);
	ib.Init(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * backpackMesh.indices.size(), backpackMesh.indices.size(), backpackMesh.indices.data());

	Renderable obj;
	obj.uModel = glm::mat4(1.f);
	obj.SetVertexData(&vb, &ib);
	Shader objShader;

	char vertSrc[256];
	char fragSrc[256];
	FileReader::ReadFile(SHADERS_PATH"default_vert.glsl", vertSrc);
	FileReader::ReadFile(SHADERS_PATH"default_frag.glsl", fragSrc);

	objShader.InitAndCompile(vertSrc, fragSrc);
	obj.SetShader(&objShader);

	renderer.AddRenderable(&obj);

	while (isRunning)
	{
		Time::UpdateTime();
		PollEvents(Time::deltaTime);

		Input::Update();
		renderer.camera.Update(Time::deltaTime);

		// TODO: update logic here
		renderer.Render();
	}
}