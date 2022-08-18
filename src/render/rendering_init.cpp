#include "stdafx.h"
#include "render/rendering_resources.h"
#include "render/renderer.h"
#include <glad/glad.h>
#include "math/math.h"
#include "random/random.h"
#include "debugging/gl.h"

namespace Ty
{
	namespace Graphics
	{
		void RenderPass_GBuffer::init(RenderTarget* rt)
		{
			this->rt = rt;

			rt->setOutput(g_gBuffer.diffuse, 0);
			rt->setOutput(g_gBuffer.position, 1);
			rt->setOutput(g_gBuffer.normal, 2);
		}

		void RenderPass_SSAO::init(RenderTarget* rt)
		{
			this->rt = rt;

			generateKernel();
			generateNoise();

			ssao_noiseTexture = g_textureResourceManager.createTexture({ 4, 4, TextureFormat::R32_G32_B32_FLOAT }, &ssao_data.ssaoNoise[0]);
			ssao_outputTexture = g_textureResourceManager.createTexture({ GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT, TextureFormat::R8_FLOAT }, nullptr);

			ResourceHandle<Shader> ssao_vs = g_shaderResourceManager.getFromFile(SHADERS_PATH"screen_quad_vs.vert");
			ResourceHandle<Shader> ssao_ps = g_shaderResourceManager.getFromFile(SHADERS_PATH"ssao_ps.frag");
			ShaderPipeline ssao_shaderPipeline = g_shaderResourceManager.createLinkedShaderPipeline(ssao_vs, ssao_ps);
			ssao_material.init(ssao_shaderPipeline);

			bindKernel();
			bindNoiseTexture();
			bindRadius();
			ssao_material.addTextureToSlot(g_gBuffer.position, 0);
			ssao_material.addTextureToSlot(g_gBuffer.normal, 1);
			ssao_material.addTextureToSlot(ssao_noiseTexture, 2);

			rt->setOutput(ssao_outputTexture, 0);
		}

		void RenderPass_Blur::setInputTexture(ResourceHandle<Texture> inputTexture)
		{
			if (blur_inputTexture != inputTexture)
			{
				blur_inputTexture = inputTexture;
				blur_material.addTextureToSlot(inputTexture, 0);
			}
		}

		void RenderPass_Blur::init(RenderTarget* rt)
		{
			this->rt = rt;

			blur_outputTexture = g_textureResourceManager.createTexture({ GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT, TextureFormat::R8_FLOAT }, nullptr);

			ResourceHandle<Shader> blur_vs = g_shaderResourceManager.getFromFile(SHADERS_PATH"screen_quad_vs.vert");
			ResourceHandle<Shader> blur_ps = g_shaderResourceManager.getFromFile(SHADERS_PATH"ssao_blur_ps.frag");
			ShaderPipeline blur_shaderPipeline = g_shaderResourceManager.createLinkedShaderPipeline(blur_vs, blur_ps);
			blur_material.init(blur_shaderPipeline);

			rt->setOutput(blur_outputTexture, 0);
		}

		void RenderPass_UI::init(RenderTarget* rt)
		{
			this->rt = rt;

			ui_fpsGraph.init();
			rt->setOutput(ui_fpsGraph.fpsGraphTexture, 0);
		}

		void RenderPass_Lighting::init(RenderTarget* rt)
		{
			this->rt = rt;

			lighting_outputTexture = g_textureResourceManager.createTexture({ GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT, TextureFormat::R8_G8_B8_A8_UNORM }, nullptr);
			ResourceHandle<Shader> lighting_vs = g_shaderResourceManager.getFromFile(SHADERS_PATH"screen_quad_vs.vert");
			ResourceHandle<Shader> lighting_ps = g_shaderResourceManager.getFromFile(SHADERS_PATH"final_pass_ps.frag");
			ShaderPipeline lighting_shaderPipeline = g_shaderResourceManager.createLinkedShaderPipeline(lighting_vs, lighting_ps);
			lighting_material.init(lighting_shaderPipeline);
			lighting_material.addTextureToSlot(g_gBuffer.diffuse, 0);

			rt->setOutput(lighting_outputTexture, 0);
		}

		void RenderPass_Lighting::setInputTexture(ResourceHandle<Texture> inputTexture)
		{
			if (lighting_inputTexture != inputTexture)
			{
				lighting_inputTexture = inputTexture;
				lighting_material.addTextureToSlot(inputTexture, 0);
			}
		}

		void Renderer::createNewWindow(uint32_t width, uint32_t height, uint32_t x, uint32_t y, const char* title)
		{
			pWindow = new Window();
			pWindow->Init(width, height, x, y, title);
		}

		void Renderer::createNewRenderContext()
		{
			ASSERT(!pGlContextHandle, "Trying to create new OpenGL context when renderer already has one.");
			ASSERT(pWindow, "Trying to create new OpenGL context without window associated to renderer.");
			pGlContextHandle = SDL_GL_CreateContext(pWindow->handle);
			ASSERT(pGlContextHandle, "Failed to create new OpenGL context.");
			SDL_GL_MakeCurrent(pWindow->handle, pGlContextHandle);
		}

		void Renderer::retrieveAPIFunctionLocations()
		{
			bool result = gladLoadGLLoader(SDL_GL_GetProcAddress);
			ASSERT(result, "Failed to retrieve OpenGL API function locations using GLAD.");
		}

		void Renderer::setViewport(Math::Primitives::u32_rect viewportRect)
		{
			renderViewport = viewportRect;
			GL(glViewport(renderViewport.x, renderViewport.y, renderViewport.w, renderViewport.h));
		}

		void Renderer::setCamera(float x, float y, float z, float fov, float aspect)
		{
			camera.Init(x, y, z, fov, aspect);
		}

		void Renderer::init(uint32_t w, uint32_t h, uint32_t x, uint32_t y)
		{
			// Initializing API
			{
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

				SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
				SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

				SDL_GL_SetSwapInterval(0);	// No vsync
			}

			// Initializing window and context
			createNewWindow(w, h, x, y, APP_TITLE);
			createNewRenderContext();
			retrieveAPIFunctionLocations();

			// Initializing render resource managers
			g_bufferResourceManager.init();
			g_textureResourceManager.init();
			g_shaderResourceManager.init();

			// Initializing camera
			float camera_aspect = (float)w / (float)h;
			setCamera(DEFAULT_CAMERA_X, DEFAULT_CAMERA_Y, DEFAULT_CAMERA_Z, DEFAULT_CAMERA_FOV, camera_aspect);

			// Initializing other state
			GL(glEnable(GL_DEPTH_TEST));
			GL(glEnable(GL_CULL_FACE));
			GL(glFrontFace(GL_CCW));

			setViewport({ w, h, 0, 0 });

			// Global rendering resources
			initGlobalRenderResources();

			// Render passes
			pass_gBuffer.init(&g_rtGBuffer);
			pass_ssao.init(&g_rtSSAO);
			pass_blur.init(&g_rtBlur);
			pass_ui.init(&g_rtUI);
			pass_lighting.init(&g_rtLighting);
		}

		void Renderer::destroy()
		{
			if (pGlContextHandle)
			{
				SDL_GL_DeleteContext(pGlContextHandle);
			}
			if (pWindow) delete pWindow;
		}


	}
}