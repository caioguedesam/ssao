#include "render/renderer.h"
#include "file/file_reader.h"
#include "debugging/gl.h"
#include "random/random.h"
#include "math/math.h"

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

void Renderer::SetViewport(uint32_t width, uint32_t height, uint32_t x, uint32_t y)
{
	GL(glViewport(x, y, width, height));
}

void Renderer::SetCamera(float x, float y, float z, float fov, float aspect)
{
	camera.Init(x, y, z, fov, aspect);
}

void Renderer::Init(uint32_t windowWidth, uint32_t windowHeight, uint32_t windowX, uint32_t windowY, const char* windowTitle,
	float cameraX, float cameraY, float cameraZ, float cameraFOV, float cameraAspect)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	CreateNewWindow(windowWidth, windowHeight, windowX, windowY, windowTitle);
	CreateNewRenderContext();
	RetrieveAPIFunctionLocations();

	GL(glEnable(GL_DEPTH_TEST));
	GL(glEnable(GL_CULL_FACE));
	GL(glFrontFace(GL_CCW));

	SetViewport(windowWidth, windowHeight, 0, 0);
	SetCamera(cameraX, cameraY, cameraZ, cameraFOV, cameraAspect);

	// Initializing default resources
	defaultQuadVertexBuffer.Init(GL_ARRAY_BUFFER, sizeof(float) * defaultQuadVertices.size(), defaultQuadVertices.size(), defaultQuadVertices.data());
	defaultQuadIndexBuffer.Init(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * defaultQuadIndices.size(), defaultQuadIndices.size(), defaultQuadIndices.data());

	// Initializing geometry render target
	gDiffuseTexture.Init(windowWidth, windowHeight, Texture::Format::RGBA_UNORM, nullptr/*, Texture::CreationFlags::RENDER_TARGET*/);
	gPositionTexture.Init(windowWidth, windowHeight, Texture::Format::RGBA_16FLOAT, nullptr, Texture::Params::WRAP_CLAMP_EDGE/*, Texture::CreationFlags::RENDER_TARGET*/);
	gNormalTexture.Init(windowWidth, windowHeight, Texture::Format::RGBA_16FLOAT, nullptr/*, Texture::CreationFlags::RENDER_TARGET*/);
	RT_Geometry.Init(windowWidth, windowHeight, &gDiffuseTexture);
	RT_Geometry.AddTextureToSlot(&gPositionTexture, 1);
	RT_Geometry.AddTextureToSlot(&gNormalTexture, 2);

	// Initializing post-processing resources and render target
	for (int i = 0; i < 64; i++)	// 64 points for kernel
	{
		glm::vec3 sample(
			Random::UniformDistribution(-1.f, 1.f),
			Random::UniformDistribution(-1.f, 1.f),
			Random::UniformDistribution(0.f, 1.f)
		);
		sample = glm::normalize(sample);
		sample *= Random::UniformDistribution();
		// Push sample towards center
		float scale = (float)i / 64.f;
		scale = Math::Lerp(0.1f, 1.f, scale * scale);
		sample *= scale;

		ssaoKernel.push_back(sample);
	}

	std::vector<glm::vec3> ssaoNoise;
	for (int i = 0; i < 16; i++)
	{
		glm::vec3 noise(
			Random::UniformDistribution(-1.f, 1.f),
			Random::UniformDistribution(-1.f, 1.f),
			0.f
		);
		ssaoNoise.push_back(noise);
	}
	// TODO_#SSAO: Upload this texture and kernel to rest of pipeline
	ssaoNoiseTexture.Init(4, 4, Texture::Format::RGBA_16FLOAT, &ssaoNoise[0]);
	RT_SSAO.Init(windowWidth, windowHeight, &gPositionTexture);
	// TODO_#SSAO: add other textures, continue from here

	char vertSrc[1024];
	char fragSrc[1024];
	FileReader::ReadFile(SHADERS_PATH"second_pass_vert.glsl", vertSrc);
	FileReader::ReadFile(SHADERS_PATH"second_pass_frag.glsl", fragSrc);
	screenQuadShader.InitAndCompile(vertSrc, fragSrc);

	screenQuadMaterial.Init(&screenQuadShader);
	screenQuadMaterial.AddTextureToSlot(&rtDiffuseTexture, 0);
	screenQuadMaterial.AddTextureToSlot(&rtPositionTexture, 1);
	screenQuadMaterial.AddTextureToSlot(&rtNormalTexture, 2);

	screenQuad.SetVertexData(&defaultQuadVertexBuffer, &defaultQuadIndexBuffer);
	screenQuad.SetMaterial(&screenQuadMaterial);
}

void Renderer::Destroy()
{
	//if (screenQuadShader) delete screenQuadShader;
	//if (screenQuadMaterial) delete screenQuadMaterial;
	//if (screenQuad) delete screenQuad;
}

void Renderer::OnResize(uint32_t newWidth, uint32_t newHeight)
{
	pWindow->OnResize(newWidth, newHeight);
	SetViewport(newWidth, newHeight, 0, 0);
}

void Renderer::SetVertexBuffer(Buffer* buffer, uint32_t elementSize, uint32_t elementStride, uint32_t firstElementOffset)
{
	buffer->Bind(GL_ARRAY_BUFFER);
	// TODO: Currently vertex attributes are hard set.
	uint32_t offset = firstElementOffset * elementSize;
	GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * elementSize, (void*)offset));
	GL(glEnableVertexAttribArray(0));
}

void Renderer::SetIndexBuffer(Buffer* buffer, uint32_t elementSize, uint32_t elementStride, uint32_t firstElementOffset)
{
	buffer->Bind(GL_ELEMENT_ARRAY_BUFFER);
}

void Renderer::AddRenderable(Renderable* renderable)
{
	renderables.push_back(renderable);
}

void Renderer::Render()
{
	// First pass
	rt.Bind();
	GL(glEnable(GL_DEPTH_TEST));
	GL(glClearColor(0.f, 0.f, 0.f, 1.f));
	GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	for (int i = 0; i < renderables.size(); i++)
	{
		Renderable* renderable = renderables[i];
		RenderParams params;
		params.model = renderable->uModel;
		params.view = camera.GetViewMatrix();
		params.proj = camera.GetProjectionMatrix();

		renderable->Draw(params);
	}

	// Second pass
	rt.Unbind();
	GL(glDisable(GL_DEPTH_TEST));
	GL(glClearColor(1.f, 1.f, 1.f, 1.f));
	RenderParams params;
	screenQuad.Draw(params);

	SDL_GL_SwapWindow(pWindow->handle);
}