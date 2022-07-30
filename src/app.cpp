#include "stdafx.h"
#include "app.h"

#include <SDL.h>
#include "glm/gtx/transform.hpp"

#include "time/time.h"
#include "resource/resource_loader.h"
#include "resource/buffer_resource_manager.h"
#include "resource/texture_resource_manager.h"
#include "resource/shader_resource_manager.h"
#include "render/shader_compiler.h"
#include "gui/gui.h"

// TODO_MULTIPLATFORM: Remove this SDL dependency and use platform api
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

	// Core engine systems initialization
	Time::Init();
	Input::Init();

	// Rendering system initialization
	renderer.Init(appWidth, appHeight, (g_screenWidth - appWidth) / 2, (g_screenHeight - appHeight) / 2, "SSAO",
		0, 0, 3.f, 45.f, static_cast<float>(appWidth) / static_cast<float>(appHeight));

	// Resource system initialization
	g_textureResourceManager.init();
	g_shaderResourceManager.init();
	renderer.initializeRenderResources(appWidth, appHeight);

	GUI::init(this);

	isRunning = true;
}

void App::PollEvents(double dt)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		GUI::processEvent(&event);
		if (event.type == SDL_KEYDOWN)
		{
			SDL_Keysym key = event.key.keysym;
			switch (key.sym)
			{
			case SDLK_ESCAPE:
			{
				isRunning = false;
			} break;
			case SDLK_p:
			{
				Input::ChangeInputMode(Input::currentState == Input::State::APP ? Input::State::GUI : Input::State::APP);
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
				//renderer.OnResize(newWidth, newHeight);	// TODO_RESIZE: Renderer shouldn't be resizing (default 1080p). What has to resize is the gui window w/ final texture.
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
	Model sponza;
	ResourceLoader::LoadModel(sponza, MODELS_PATH"sponza.obj");

	ResourceHandle<Buffer> vertexBuffer = g_bufferResourceManager.createBuffer({ BufferType::VERTEX_BUFFER, BufferFormat::R32_FLOAT, sponza.vertices.size() }, sponza.vertices.data());
	ResourceHandle<Buffer> indexBuffer = g_bufferResourceManager.createBuffer({ BufferType::INDEX_BUFFER, BufferFormat::R32_UINT, sponza.indices.size() }, sponza.indices.data());

	Renderable obj;
	obj.uModel = glm::mat4(1.f);
	obj.uModel = glm::scale(obj.uModel, glm::vec3(0.01f, 0.01f, 0.01f));
	obj.setVertexData(vertexBuffer, indexBuffer);

	ResourceHandle<Shader> vs_obj = g_shaderResourceManager.getFromFile(SHADERS_PATH"default_vs.vert");
	ResourceHandle<Shader> ps_obj = g_shaderResourceManager.getFromFile(SHADERS_PATH"default_ps.frag");
	ShaderPipeline objShaderPipeline = g_shaderResourceManager.createLinkedShaderPipeline(vs_obj, ps_obj);

	Material objMat;
	objMat.init(objShaderPipeline);

	obj.setMaterial(&objMat);

	renderer.AddRenderable(&obj);

	while (isRunning)
	{
		Time::UpdateTime();
		PollEvents(Time::deltaTime);

		Input::Update();
		renderer.camera.Update(Time::deltaTime);

		// TODO: update logic here
		renderer.fpsGraph.setFrameData(Time::getLastFrameTimes(), Time::getLastTrackedFrame());

		renderer.Render();

		GUI::display(this);

		renderer.Flush();
	}
}