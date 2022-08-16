#pragma once
#include "stdafx.h"

#include <SDL.h>

#include "display/window.h"

#include "resource/texture_resource_manager.h"

#include "render/camera.h"
#include "render/renderable.h"
#include "render/render_target.h"
#include "gui/fps_window.h"
#include "math/math.h"

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

class Renderer
{
public:
	Window* pWindow;
	SDL_GLContext pGlContextHandle;
	Math::Primitives::u32_rect renderViewport;
	Camera camera;

	// Render passes
	RenderPass_GBuffer		pass_gBuffer;
	RenderPass_SSAO			pass_ssao;
	RenderPass_Blur			pass_blur;
	RenderPass_UI			pass_ui;
	RenderPass_Lighting		pass_lighting;

	void createNewWindow(uint32_t width, uint32_t height, uint32_t x, uint32_t y, const char* title);
	void createNewRenderContext();
	void retrieveAPIFunctionLocations();

	void setViewport(Math::Primitives::u32_rect viewportRect);
	void setCamera(float x, float y, float z, float fov, float aspect);

	void init(uint32_t w, uint32_t h, uint32_t x, uint32_t y);
	void destroy();

	void clear(const float& r = 0.0f, const float& g = 0.0f, const float& b = 0.0f, const float& a = 0.0f);
	void render();
	void flush();
};