#pragma once
#include "stdafx.h"

#include <SDL.h>

#include "display/window.h"
#include "render/camera.h"
#include "render/renderable.h"
#include "render/render_target.h"

class Renderer
{
public:
	Window* pWindow;
	SDL_GLContext pGlContextHandle;
	Camera camera;

	std::vector<Renderable*> renderables;

	// Default render resources
	Buffer defaultQuadVertexBuffer;
	Buffer defaultQuadIndexBuffer;
	Renderable screenQuad;

	// G-Buffer Pass
	RenderTarget RT_Geometry;

	Texture gPositionTexture;
	Texture gNormalTexture;
	Texture gDiffuseTexture;

	// Post-processing pass (SSAO)
	RenderTarget RT_SSAO;
	RenderTarget RT_Blur;

	std::vector<glm::vec3> ssaoKernel;	// Random points distributed between a unit hemisphere (biased towards center)
	Texture ssaoNoiseTexture;			// Random rotation texture to introduce randomness when using SSAO kernel

	Texture ssaoResultTexture;
	Texture ssaoBlurTexture;
	Material ssaoMaterial;
	Material ssaoBlurMaterial;
	Shader ssaoShader;
	Shader ssaoBlurShader;
	bool enableBlurPass = true;

	// Final pass (uses default screen quad)
	Material finalPassMaterial;
	Shader finalPassShader;

	~Renderer();

	void CreateNewWindow(uint32_t width, uint32_t height, uint32_t x, uint32_t y, const char* title);

	void CreateNewRenderContext();

	void RetrieveAPIFunctionLocations();

	void SetViewport(uint32_t width, uint32_t height, uint32_t x, uint32_t y);

	void SetCamera(float x, float y, float z, float fov, float aspect);

	void InitPostProcessResources(uint32_t windowWidth, uint32_t windowHeight);

	void Init(uint32_t windowWidth, uint32_t windowHeight, uint32_t windowX, uint32_t windowY, const char* windowTitle,
		float cameraX, float cameraY, float cameraZ, float cameraFOV, float cameraAspect);

	void Destroy();

	void OnResize(uint32_t newWidth, uint32_t newHeight);

	void SetVertexBuffer(Buffer* buffer, uint32_t elementSize, uint32_t elementStride, uint32_t firstElementOffset);

	void SetIndexBuffer(Buffer* buffer, uint32_t elementSize, uint32_t elementStride, uint32_t firstElementOffset);

	void AddRenderable(Renderable* renderable);

	void Render();

	void Flush();
};