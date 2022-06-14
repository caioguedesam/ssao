#include "stdafx.h"
#include "app.h"

#include <SDL.h>
#include "glm/gtx/transform.hpp"

#include "time/time.h"
#include "resource/resource_loader.h"
#include "render/shader_compiler.h"
#include "gui/gui.h"

// TODO_#MULTIPLATFORM: Remove this SDL dependency and use platform api
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
	GUI::Init(&renderer);

	isRunning = true;
}

void App::PollEvents(double dt)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		GUI::ProcessSDLEvent(&event);
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
				renderer.OnResize(newWidth, newHeight);
			}
		}
		else if (event.type == SDL_QUIT)
		{
			isRunning = false;
		}
	}
}

void App::DisplayGUI()
{
	GUI::BeginFrame();
	{
		// Test GUI code
		GUI::BeginWindow("Debug", 320, 150, 0, 0);
		char fps_str[64];
		sprintf(fps_str, "FPS: %.1lf", Time::fps);
		GUI::Text(fps_str);
		GUI::Checkbox("Enable Blur", &renderer.enableBlurPass);

		int oldSsaoKernelSize = renderer.ssaoData.ssaoKernelSize;
		GUI::Slider_int("Kernel size", &renderer.ssaoData.ssaoKernelSize, 0, MAX_SSAO_KERNEL_SIZE);
		if (oldSsaoKernelSize != renderer.ssaoData.ssaoKernelSize)
		{
			renderer.ssaoData.GenerateKernel();
			renderer.ssaoData.BindKernel(&renderer.ssaoShader);
		}

		int oldSsaoKernelDimension = renderer.ssaoData.ssaoNoiseDimension;
		GUI::Slider_int("Noise dimension", &renderer.ssaoData.ssaoNoiseDimension, 0, MAX_SSAO_NOISE_DIMENSION);
		if (oldSsaoKernelDimension != renderer.ssaoData.ssaoNoiseDimension)
		{
			renderer.ssaoData.GenerateNoise();
			renderer.ssaoData.BindNoiseTexture(&renderer.ssaoShader, &renderer.ssaoNoiseTexture);
		}

		int oldRadius = renderer.ssaoData.ssaoRadius;
		GUI::Slider_float("Radius", &renderer.ssaoData.ssaoRadius, 0, MAX_SSAO_RADIUS);
		if (oldRadius != renderer.ssaoData.ssaoRadius)
		{
			renderer.ssaoData.BindRadius(&renderer.ssaoShader);
		}

		GUI::EndWindow();
	}
	GUI::EndFrame();
}

void App::Run()
{
	/*Model backpackModel;
	ResourceLoader::LoadModel(backpackModel, MODELS_PATH"backpack.obj");
	Texture backpackDiffuseMap;
	Texture backpackNormalMap;
	ResourceLoader::LoadTexture(backpackDiffuseMap, TEXTURES_PATH"backpack_diffuse.jpg");
	ResourceLoader::LoadTexture(backpackNormalMap, TEXTURES_PATH"backpack_normal.png");*/

	Model sponza;
	ResourceLoader::LoadModel(sponza, MODELS_PATH"sponza.obj");

	/*Buffer vb;
	vb.Init(GL_ARRAY_BUFFER, sizeof(float) * backpackModel.vertices.size(), backpackModel.vertices.size(), backpackModel.vertices.data());
	Buffer ib;
	ib.Init(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * backpackModel.indices.size(), backpackModel.indices.size(), backpackModel.indices.data());*/
	Buffer vb;
	vb.Init(GL_ARRAY_BUFFER, sizeof(float) * sponza.vertices.size(), sponza.vertices.size(), sponza.vertices.data());
	Buffer ib;
	ib.Init(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * sponza.indices.size(), sponza.indices.size(), sponza.indices.data());

	Renderable obj;
	obj.uModel = glm::mat4(1.f);
	obj.uModel = glm::scale(obj.uModel, glm::vec3(0.01f, 0.01f, 0.01f));
	obj.SetVertexData(&vb, &ib);
	Shader objShader;

	ShaderCompiler::CompileAndLinkShader(&objShader, 
		SHADERS_PATH"default_vs.vert",
		SHADERS_PATH"default_ps.frag");

	Material objMat;
	objMat.Init(&objShader);
	/*objMat.AddTextureToSlot(&backpackDiffuseMap, 0);
	objMat.AddTextureToSlot(&backpackNormalMap, 1);*/

	obj.SetMaterial(&objMat);

	renderer.AddRenderable(&obj);

	while (isRunning)
	{
		Time::UpdateTime();
		PollEvents(Time::deltaTime);

		Input::Update();
		renderer.camera.Update(Time::deltaTime);

		// TODO: update logic here
		renderer.Render();

		DisplayGUI();

		renderer.Flush();
	}
}