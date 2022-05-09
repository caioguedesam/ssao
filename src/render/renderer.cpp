#include "render/renderer.h"

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
	glViewport(x, y, width, height);
	ASSERT(glGetError() == GL_NO_ERROR, "");
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

	glEnable(GL_DEPTH_TEST);
	ASSERT(glGetError() == GL_NO_ERROR, "");
	glEnable(GL_CULL_FACE);
	ASSERT(glGetError() == GL_NO_ERROR, "");
	glFrontFace(GL_CCW);
	ASSERT(glGetError() == GL_NO_ERROR, "");

	SetViewport(windowWidth, windowHeight, 0, 0);
	SetCamera(cameraX, cameraY, cameraZ, cameraFOV, cameraAspect);
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * elementSize, (void*)offset);
	glEnableVertexAttribArray(0);
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
	glClearColor(0.f, 0.f, 0.f, 1.f);
	ASSERT(glGetError() == GL_NO_ERROR, "");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ASSERT(glGetError() == GL_NO_ERROR, "");

	for (int i = 0; i < renderables.size(); i++)
	{
		Renderable* renderable = renderables[i];
		RenderParams params;
		params.model = renderable->uModel;
		params.view = camera.GetViewMatrix();
		params.proj = camera.GetProjectionMatrix();

		renderable->Draw(params);
	}

	SDL_GL_SwapWindow(pWindow->handle);
}