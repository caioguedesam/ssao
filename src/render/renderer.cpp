#include "stdafx.h"
#include "render/renderer.h"
#include "debugging/gl.h"
#include "random/random.h"
#include "math/math.h"
#include "resource/buffer_resource_manager.h"
#include <glad/glad.h>

std::vector<float> defaultQuadVertices =
{
	// position			// normal			// uvs
	-1.f, -1.f, 0.f,	0.f, 0.f, 0.f,		0.f, 0.f,
	-1.f, 1.f, 0.f,		0.f, 0.f, 0.f,		0.f, 1.f,
	1.f, 1.f, 0.f,		0.f, 0.f, 0.f,		1.f, 1.f,
	1.f, -1.f, 0.f,		0.f, 0.f, 0.f,		1.f, 0.f,
};

std::vector<uint32_t> defaultQuadIndices =
{
	0, 2, 1,
	0, 3, 2,
};

void SSAOData::GenerateKernel()
{
	for (int i = 0; i < ssaoKernelSize; i++)	// 64 points for kernel
	{
		glm::vec3 sample(
			Random::UniformDistribution(-1.f, 1.f),
			Random::UniformDistribution(-1.f, 1.f),
			Random::UniformDistribution(0.f, 1.f)
		);
		sample = glm::normalize(sample);
		sample *= Random::UniformDistribution();
		// Push sample towards center
		float scale = float(i) / 64.f;
		scale = Math::Lerp(0.1f, 1.f, scale * scale);
		sample *= scale;

		//ssaoKernel.push_back(sample);
		ssaoKernel[i] = sample;
	}
}

void SSAOData::bindKernel(ShaderPipeline& shaderPipeline)
{
	shaderPipeline.bind();
	char ssaoKernelName[32];
	for (int i = 0; i < ssaoKernelSize; i++)
	{
		sprintf(ssaoKernelName, "samples[%d]", i);
		shaderPipeline.setUniform(ssaoKernelName, ssaoKernel[i]);
	}
	shaderPipeline.setUniform("kernelSize", ssaoKernelSize);
}

void SSAOData::GenerateNoise()
{
	for (int i = 0; i < ssaoNoiseDimension * ssaoNoiseDimension; i++)
	{
		glm::vec3 noise(
			Random::UniformDistribution(-1.f, 1.f),
			Random::UniformDistribution(-1.f, 1.f),
			0.f
		);
		ssaoNoise[i] = noise;
	}
}

void SSAOData::bindNoiseTexture(ShaderPipeline& shaderPipeline, ResourceHandle<Texture> textureHandle)
{
	shaderPipeline.bind();
	shaderPipeline.setUniform("noiseDimension", ssaoNoiseDimension);
	g_textureResourceManager.updateTexture(textureHandle, 
		{ 
			static_cast<uint32_t>(ssaoNoiseDimension), 
			static_cast<uint32_t>(ssaoNoiseDimension),
			TextureFormat::R32_G32_B32_FLOAT
		},
		&ssaoNoise[0]);
}

void SSAOData::bindRadius(ShaderPipeline& shaderPipeline)
{
	shaderPipeline.bind();
	shaderPipeline.setUniform("radius", ssaoRadius);
}

RenderViewport::RenderViewport() {}
RenderViewport::RenderViewport(const uint32_t& width, const uint32_t& height, const uint32_t& x, const uint32_t& y)
	: width(width), height(height), x(x), y(y) {}

void RenderViewport::set()
{
	GL(glViewport(x, y, width, height));
}

Renderer::~Renderer()
{
	if (pGlContextHandle)
	{
		SDL_GL_DeleteContext(pGlContextHandle);
	}
	if (pWindow) delete pWindow;
}

void Renderer::CreateNewWindow(uint32_t width, uint32_t height, uint32_t x, uint32_t y, const char* title)
{
	pWindow = new Window();
	pWindow->Init(width, height, x, y, title);
}

void Renderer::CreateNewRenderContext()
{
	ASSERT(!pGlContextHandle, "Trying to create new OpenGL context when renderer already has one.");
	ASSERT(pWindow, "Trying to create new OpenGL context without window associated to renderer.");
	pGlContextHandle = SDL_GL_CreateContext(pWindow->handle);
	ASSERT(pGlContextHandle, "Failed to create new OpenGL context.");
	SDL_GL_MakeCurrent(pWindow->handle, pGlContextHandle);
}

void Renderer::RetrieveAPIFunctionLocations()
{
	bool result = gladLoadGLLoader(SDL_GL_GetProcAddress);
	ASSERT(result, "Failed to retrieve OpenGL API function locations using GLAD.");
}

void Renderer::setViewport()
{
	renderViewport.set();
}

void Renderer::setViewport(uint32_t w, uint32_t h, uint32_t x, uint32_t y)
{
	renderViewport = RenderViewport(w, h, x, y);
	renderViewport.set();
}

void Renderer::setViewport(RenderViewport viewport)
{
	renderViewport = viewport;
	setViewport();
}

void Renderer::SetCamera(float x, float y, float z, float fov, float aspect)
{
	camera.Init(x, y, z, fov, aspect);
}

void Renderer::InitPostProcessResources(uint32_t windowWidth, uint32_t windowHeight)
{
	ssaoData.GenerateKernel();
	ssaoData.GenerateNoise();

	ssaoNoiseTexture = g_textureResourceManager.createTexture({ 4, 4, TextureFormat::R32_G32_B32_FLOAT }, &ssaoData.ssaoNoise[0]);
	ssaoResultTexture = g_textureResourceManager.createTexture({ windowWidth, windowHeight, TextureFormat::R8_FLOAT }, nullptr);
	ssaoBlurTexture = g_textureResourceManager.createTexture({ windowWidth, windowHeight, TextureFormat::R8_FLOAT }, nullptr);
}

void Renderer::Init(uint32_t windowWidth, uint32_t windowHeight, uint32_t windowX, uint32_t windowY, const char* windowTitle,
	float cameraX, float cameraY, float cameraZ, float cameraFOV, float cameraAspect)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SDL_GL_SetSwapInterval(0);	// No vsync

	CreateNewWindow(windowWidth, windowHeight, windowX, windowY, windowTitle);
	CreateNewRenderContext();
	RetrieveAPIFunctionLocations();

	GL(glEnable(GL_DEPTH_TEST));
	GL(glEnable(GL_CULL_FACE));
	GL(glFrontFace(GL_CCW));

	setViewport(windowWidth, windowHeight, 0, 0);
	SetCamera(cameraX, cameraY, cameraZ, cameraFOV, cameraAspect);
}

void Renderer::initializeRenderResources(uint32_t windowWidth, uint32_t windowHeight)
{
	// Initializing default resources
	defaultQuadVertexBuffer = g_bufferResourceManager.createBuffer(
		{
			BufferType::VERTEX_BUFFER,
			BufferFormat::R32_FLOAT,
			defaultQuadVertices.size()
		}, defaultQuadVertices.data()
	);
	defaultQuadIndexBuffer = g_bufferResourceManager.createBuffer(
		{
			BufferType::INDEX_BUFFER,
			BufferFormat::R32_UINT,
			defaultQuadIndices.size()
		}, defaultQuadIndices.data()
	);
	screenQuad.setVertexData(defaultQuadVertexBuffer, defaultQuadIndexBuffer);

	// Initializing G-Buffer pass resources
	gDiffuseTexture = g_textureResourceManager.createTexture({ windowWidth, windowHeight, TextureFormat::R8_G8_B8_A8_UNORM }, nullptr);
	gPositionTexture = g_textureResourceManager.createTexture({ windowWidth, windowHeight, TextureFormat::R16_G16_B16_A16_FLOAT, TextureParams::WRAP_CLAMP_EDGE }, nullptr);
	gNormalTexture = g_textureResourceManager.createTexture({ windowWidth, windowHeight, TextureFormat::R16_G16_B16_A16_FLOAT }, nullptr);

	RT_Geometry.Init(windowWidth, windowHeight, gDiffuseTexture);
	RT_Geometry.SetOutputTexture(gPositionTexture, 1);
	RT_Geometry.SetOutputTexture(gNormalTexture, 2);

	// Initializing Post-process pass (SSAO) resources
	InitPostProcessResources(windowWidth, windowHeight);
	RT_SSAO.Init(windowWidth, windowHeight, ssaoResultTexture);
	RT_Blur.Init(windowWidth, windowHeight, ssaoBlurTexture);

	// SSAO pass resources
	{
		ResourceHandle<Shader> vs_ssao = g_shaderResourceManager.getFromFile(SHADERS_PATH"screen_quad_vs.vert");
		ResourceHandle<Shader> ps_ssao = g_shaderResourceManager.getFromFile(SHADERS_PATH"ssao_ps.frag");
		ShaderPipeline ssaoShaderPipeline = g_shaderResourceManager.createLinkedShaderPipeline(vs_ssao, ps_ssao);
		ssaoMaterial.init(ssaoShaderPipeline);

		ssaoData.bindKernel(ssaoMaterial.shaderPipeline);
		ssaoData.bindNoiseTexture(ssaoMaterial.shaderPipeline, ssaoNoiseTexture);
		ssaoData.bindRadius(ssaoMaterial.shaderPipeline);
		ssaoMaterial.addTextureToSlot(gPositionTexture, 0);
		ssaoMaterial.addTextureToSlot(gNormalTexture, 1);
		ssaoMaterial.addTextureToSlot(ssaoNoiseTexture, 2);
	}

	// Blur pass resources
	{
		ResourceHandle<Shader> vs_ssaoBlur = g_shaderResourceManager.getFromFile(SHADERS_PATH"screen_quad_vs.vert");
		ResourceHandle<Shader> ps_ssaoBlur = g_shaderResourceManager.getFromFile(SHADERS_PATH"ssao_blur_ps.frag");
		ShaderPipeline ssaoBlurShaderPipeline = g_shaderResourceManager.createLinkedShaderPipeline(vs_ssaoBlur, ps_ssaoBlur);
		ssaoBlurMaterial.init(ssaoBlurShaderPipeline);
		ssaoBlurMaterial.addTextureToSlot(ssaoResultTexture, 0);
	}

	// GUI pass resources
	{
		fpsGraph.init();
		RT_FpsGraph.Init(FPS_WINDOW_WIDTH, FPS_WINDOW_HEIGHT, fpsGraph.fpsGraphTexture);
	}

	// Final pass resources
	{
		finalPassTexture = g_textureResourceManager.createTexture({ windowWidth, windowHeight, TextureFormat::R8_G8_B8_A8_UNORM }, nullptr);
		ResourceHandle<Shader> vs_final = g_shaderResourceManager.getFromFile(SHADERS_PATH"screen_quad_vs.vert");
		ResourceHandle<Shader> ps_final = g_shaderResourceManager.getFromFile(SHADERS_PATH"final_pass_ps.frag");
		ShaderPipeline finalPassShaderPipeline = g_shaderResourceManager.createLinkedShaderPipeline(vs_final, ps_final);
		finalPassMaterial.init(finalPassShaderPipeline);
		finalPassMaterial.addTextureToSlot(gDiffuseTexture, 0);
		finalPassMaterial.addTextureToSlot(ssaoBlurTexture, 1);
		RT_Final.Init(windowWidth, windowHeight, finalPassTexture);
	}
}

void Renderer::Destroy() {}

//void Renderer::OnResize(uint32_t newWidth, uint32_t newHeight)
//{
//	pWindow->OnResize(newWidth, newHeight);
//	setViewport(newWidth, newHeight, 0, 0);
//}

void Renderer::AddRenderable(Renderable* renderable)
{
	renderables.push_back(renderable);
}

//void Renderer::clear(const float& r, const float& g, const float& b, const float& a)
//{
//	GL(glClearColor(r, g, b, a));
//	GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
//}

void Renderer::Render()
{
	//ShaderCompiler::ReloadDirtyShaders();	// TODO_SHADER: Reimplement this using new shader resource manager

	// Pre-render configuration
	{
		setViewport();
		GL(glEnable(GL_DEPTH_TEST));
		clear(0.169f, 0.169f, 0.169f, 1.f);		// This clears background
	}

	// G-Buffer pass
	RenderParams params;
	{
		RT_Geometry.Bind();
		clear();								// This clears geometry render target
		params.view = camera.GetViewMatrix();
		params.proj = camera.GetProjectionMatrix();
		for (int i = 0; i < renderables.size(); i++)
		{
			Renderable* renderable = renderables[i];
			params.model = renderable->uModel;

			renderable->draw(params);
		}
		RT_Geometry.Unbind();
	}

	// Post-processing pass (SSAO + blur)
	{
		RT_SSAO.Bind();
		GL(glDisable(GL_DEPTH_TEST));
		GL(glClearColor(1.f, 1.f, 1.f, 1.f));
		params.model = glm::mat4(1.f);
		screenQuad.setMaterial(&ssaoMaterial);
		screenQuad.draw(params);
		RT_SSAO.Unbind();
	}

	if (enableBlurPass)	//TODO_RENDER: Improve render pass architecture
	{
		RT_Blur.Bind();
		GL(glDisable(GL_DEPTH_TEST));
		GL(glClearColor(1.f, 1.f, 1.f, 1.f));
		params.model = glm::mat4(1.f);
		screenQuad.setMaterial(&ssaoBlurMaterial);
		screenQuad.draw(params);
		RT_Blur.Unbind();
	}

	// GUI pass
	{
		RT_FpsGraph.Bind();
		clear();
		RenderViewport temp = renderViewport;
		RenderViewport fpsGraphViewport(FPS_WINDOW_WIDTH, FPS_WINDOW_HEIGHT, 0, 0);
		setViewport(fpsGraphViewport);

		fpsGraph.update();
		fpsGraph.fpsGraphImmRenderable.draw(params);

		RT_FpsGraph.Unbind();
		setViewport(temp);
	}

	// Final pass
	{
		RT_Final.Bind();
		finalPassMaterial.addTextureToSlot(enableBlurPass ? ssaoBlurTexture : ssaoResultTexture, 1);	//TODO_#CUSTOMIZE_RENDER_PASS: For the love of god improve this
		screenQuad.setMaterial(&finalPassMaterial);
		screenQuad.draw(params);
		RT_Final.Unbind();
	}
}

//void Renderer::Flush()
//{
//	// Swap buffers
//	SDL_GL_SwapWindow(pWindow->handle);
//}