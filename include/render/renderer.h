#pragma once
#include "stdafx.h"

#include <SDL.h>

#include "display/window.h"

#include "resource/texture_resource_manager.h"

#include "render/camera.h"
#include "render/renderable.h"
#include "render/render_target.h"
#include "gui/fps_window.h"

#define MAX_SSAO_KERNEL_SIZE 256
#define MAX_SSAO_NOISE_DIMENSION 64
#define MAX_SSAO_RADIUS 5

struct SSAOData
{
	glm::vec3 ssaoKernel[MAX_SSAO_KERNEL_SIZE];
	int ssaoKernelSize = 64;
	glm::vec3 ssaoNoise[MAX_SSAO_NOISE_DIMENSION * MAX_SSAO_NOISE_DIMENSION];
	int ssaoNoiseDimension = 4;
	float ssaoRadius = 0.5f;
};

class Renderer;

struct RenderPass
{
	bool enabled = true;
	RenderTarget* rt = nullptr;
	
	virtual void init(RenderTarget* rt) = 0;
	virtual void pass(Renderer* renderer) = 0;
};

struct RenderPass_GBuffer : RenderPass
{
	std::vector<Renderable*> renderables;

	void init(RenderTarget* rt) override;
	void pass(Renderer* renderer) override;

	void addRenderable(Renderable* renderable);
};

struct RenderPass_SSAO : RenderPass
{
	SSAOData				ssao_data;
	Material				ssao_material;
	ResourceHandle<Texture> ssao_noiseTexture;
	ResourceHandle<Texture> ssao_outputTexture;

	void init(RenderTarget* rt) override;
	void pass(Renderer* renderer) override;

	void generateKernel();
	void bindKernel();
	void generateNoise();
	void bindNoiseTexture();
	void bindRadius();
};

struct RenderPass_Blur : RenderPass
{
	Material				blur_material;
	ResourceHandle<Texture> blur_inputTexture;
	ResourceHandle<Texture> blur_outputTexture;

	void init(RenderTarget* rt) override;
	void pass(Renderer* renderer) override;

	void setInputTexture(ResourceHandle<Texture> inputTexture);
};

struct RenderPass_UI : RenderPass
{
	FPSGraph ui_fpsGraph;

	void init(RenderTarget* rt) override;
	void pass(Renderer* renderer) override;
};

struct RenderPass_Lighting : RenderPass
{
	Material				lighting_material;
	ResourceHandle<Texture> lighting_inputTexture;
	ResourceHandle<Texture> lighting_outputTexture;

	void init(RenderTarget* rt) override;
	void pass(Renderer* renderer) override;

	void setInputTexture(ResourceHandle<Texture> inputTexture);
};

struct RenderViewport
{
	uint32_t width = 0;
	uint32_t height = 0;
	uint32_t x = 0;
	uint32_t y = 0;

	RenderViewport();
	RenderViewport(const uint32_t& width, const uint32_t& height, const uint32_t& x, const uint32_t& y);
	void set();
};

class Renderer
{
public:
	Window* pWindow;
	SDL_GLContext pGlContextHandle;
	RenderViewport renderViewport;
	Camera camera;

	//std::vector<Renderable*> renderables;

	//// Default render resources
	//ResourceHandle<Buffer> defaultQuadVertexBuffer;
	//ResourceHandle<Buffer> defaultQuadIndexBuffer;
	//Renderable screenQuad;

	//// G-Buffer Pass
	//RenderTarget RT_Geometry;

	//ResourceHandle<Texture> gPositionTexture;
	//ResourceHandle<Texture> gNormalTexture;
	//ResourceHandle<Texture> gDiffuseTexture;

	//// Post-processing pass (SSAO)
	//RenderTarget RT_SSAO;
	//RenderTarget RT_Blur;

	//SSAOData ssaoData;
	//ResourceHandle<Texture> ssaoNoiseTexture;			// Random rotation texture to introduce randomness when using SSAO kernel

	//ResourceHandle<Texture> ssaoResultTexture;
	//ResourceHandle<Texture> ssaoBlurTexture;
	//Material ssaoMaterial;
	//Material ssaoBlurMaterial;
	//bool enableBlurPass = true;

	//// GUI pass
	//RenderTarget RT_FpsGraph;
	//FPSGraph fpsGraph;

	//// Final pass
	//RenderTarget RT_Final;

	//ResourceHandle<Texture> finalPassTexture;
	//Material finalPassMaterial;

	// Render passes
	RenderPass_GBuffer		pass_gBuffer;
	RenderPass_SSAO			pass_ssao;
	RenderPass_Blur			pass_blur;
	RenderPass_UI			pass_ui;
	RenderPass_Lighting		pass_lighting;


	~Renderer();

	void createNewWindow(uint32_t width, uint32_t height, uint32_t x, uint32_t y, const char* title);
	void createNewRenderContext();
	void retrieveAPIFunctionLocations();

	void setViewport();
	void setViewport(uint32_t w, uint32_t h, uint32_t x, uint32_t y);
	void setViewport(RenderViewport viewport);
	void setCamera(float x, float y, float z, float fov, float aspect);

	void init(uint32_t w, uint32_t h, uint32_t x, uint32_t y);
	void destroy();

	void clear(const float& r = 0.0f, const float& g = 0.0f, const float& b = 0.0f, const float& a = 0.0f);
	void render();
	void flush();


	//void InitPostProcessResources(uint32_t windowWidth, uint32_t windowHeight);

	/*void Init(uint32_t windowWidth, uint32_t windowHeight, uint32_t windowX, uint32_t windowY, const char* windowTitle,
		float cameraX, float cameraY, float cameraZ, float cameraFOV, float cameraAspect);
	void initializeRenderResources(uint32_t windowWidth, uint32_t windowHeight);*/

	//void AddRenderable(Renderable* renderable);
	//void Destroy();
	//void OnResize(uint32_t newWidth, uint32_t newHeight);		//TODO_RESIZE: Disabled until I figure out resizing again.
	/*void Render();
	void Flush();*/
};