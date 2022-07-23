#pragma once
#include "stdafx.h"

#include <SDL.h>

#include "display/window.h"

#include "resource/texture_resource_manager.h"

#include "render/camera.h"
#include "render/renderable.h"
#include "render/render_target.h"

#define MAX_SSAO_KERNEL_SIZE 256
#define MAX_SSAO_NOISE_DIMENSION 64
#define MAX_SSAO_RADIUS 5

struct SSAOData
{
	glm::vec3 ssaoKernel[MAX_SSAO_KERNEL_SIZE];
	int ssaoKernelSize = 64;
	glm::vec3 ssaoNoise[MAX_SSAO_NOISE_DIMENSION*MAX_SSAO_NOISE_DIMENSION];
	int ssaoNoiseDimension = 4;
	float ssaoRadius = 0.5f;

	void GenerateKernel();
	void bindKernel(ShaderPipeline& shaderPipeline);
	void GenerateNoise();
	void bindNoiseTexture(ShaderPipeline& shaderPipeline, ResourceHandle<Texture> noiseTextureHandle);
	void bindRadius(ShaderPipeline& shaderPipeline);
};

class Renderer
{
public:
	Window* pWindow;
	SDL_GLContext pGlContextHandle;
	Camera camera;

	std::vector<Renderable*> renderables;

	// Default render resources
	ResourceHandle<Buffer> defaultQuadVertexBuffer;
	ResourceHandle<Buffer> defaultQuadIndexBuffer;
	Renderable screenQuad;

	// G-Buffer Pass
	RenderTarget RT_Geometry;

	ResourceHandle<Texture> gPositionTexture;
	ResourceHandle<Texture> gNormalTexture;
	ResourceHandle<Texture> gDiffuseTexture;

	// Post-processing pass (SSAO)
	RenderTarget RT_SSAO;
	RenderTarget RT_Blur;

	//std::vector<glm::vec3> ssaoKernel;	// Random points distributed between a unit hemisphere (biased towards center)
	SSAOData ssaoData;
	ResourceHandle<Texture> ssaoNoiseTexture;			// Random rotation texture to introduce randomness when using SSAO kernel

	ResourceHandle<Texture> ssaoResultTexture;
	ResourceHandle<Texture> ssaoBlurTexture;
	Material ssaoMaterial;
	Material ssaoBlurMaterial;
	//ShaderPipeline ssaoShaderPipeline;
	//ShaderPipeline ssaoBlurShaderPipeline;
	bool enableBlurPass = true;

	// Final pass (uses default screen quad)
	Material finalPassMaterial;
	//Shader finalPassShader;

	~Renderer();

	void CreateNewWindow(uint32_t width, uint32_t height, uint32_t x, uint32_t y, const char* title);

	void CreateNewRenderContext();

	void RetrieveAPIFunctionLocations();

	void SetViewport(uint32_t width, uint32_t height, uint32_t x, uint32_t y);

	void SetCamera(float x, float y, float z, float fov, float aspect);

	void InitPostProcessResources(uint32_t windowWidth, uint32_t windowHeight);

	void Init(uint32_t windowWidth, uint32_t windowHeight, uint32_t windowX, uint32_t windowY, const char* windowTitle,
		float cameraX, float cameraY, float cameraZ, float cameraFOV, float cameraAspect);
	void initializeRenderResources(uint32_t windowWidth, uint32_t windowHeight);

	void Destroy();

	void OnResize(uint32_t newWidth, uint32_t newHeight);

	void AddRenderable(Renderable* renderable);

	void Render();

	void Flush();
};