#pragma once
#include <vector>

#include <SDL.h>

#include "display/window.h"
#include "render/camera.h"
#include "render/renderable.h"

class Renderer
{
public:
	Window* pWindow;
	SDL_GLContext pGlContextHandle;
	Camera camera;

	std::vector<Renderable*> renderables;

	~Renderer();

	void CreateNewWindow(uint32_t width, uint32_t height, uint32_t x, uint32_t y, const char* title);

	void CreateNewRenderContext();

	void RetrieveAPIFunctionLocations();

	void SetViewport(uint32_t width, uint32_t height, uint32_t x, uint32_t y);

	void SetCamera(float x, float y, float z, float fov, float aspect);

	void Init(uint32_t windowWidth, uint32_t windowHeight, uint32_t windowX, uint32_t windowY, const char* windowTitle,
		float cameraX, float cameraY, float cameraZ, float cameraFOV, float cameraAspect);

	void OnResize(uint32_t newWidth, uint32_t newHeight);

	void SetVertexBuffer(Buffer* buffer, uint32_t elementSize, uint32_t elementStride, uint32_t firstElementOffset);

	void SetIndexBuffer(Buffer* buffer, uint32_t elementSize, uint32_t elementStride, uint32_t firstElementOffset);

	void AddRenderable(Renderable* renderable);

	void Render();
};