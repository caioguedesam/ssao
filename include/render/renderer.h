#pragma once
#include "stdafx.h"

#include <SDL.h>

#include "display/window.h"

#include "resource/texture_resource_manager.h"

#include "render/camera.h"
#include "render/renderable.h"
#include "render/render_target.h"
#include "gui/fps_window.h"
#include "core/math.h"

#define MAX_SSAO_KERNEL_SIZE 256
#define MAX_SSAO_NOISE_DIMENSION 64
#define MAX_SSAO_RADIUS 5

#define MAX_LIGHTS 16

namespace Ty
{
	namespace Graphics
	{
		struct SSAOData
		{
			Math::v3f sample_kernel[MAX_SSAO_KERNEL_SIZE];
			int sample_amount = 64;
			Math::v3f white_noise[MAX_SSAO_NOISE_DIMENSION * MAX_SSAO_NOISE_DIMENSION];
			int white_noise_dimension = 4;
			float sample_radius = 0.5f;
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

			void add_renderable(Renderable* renderable);
		};

		struct RenderPass_SSAO : RenderPass
		{
			SSAOData					ssao_data;
			ResourceHandle<Material>	ssao_material;
			ResourceHandle<Texture>		ssao_noise_texture;
			ResourceHandle<Texture>		ssao_output_texture;

			void init(RenderTarget* rt) override;
			void pass(Renderer* renderer) override;

			void generate_sample_kernel();
			void bind_sample_kernel();
			void generate_white_noise();
			void bind_white_noise_texture();
			void bind_sample_radius();
		};

		struct RenderPass_Blur : RenderPass
		{
			ResourceHandle<Material>	blur_material;
			ResourceHandle<Texture>		blur_input_texture;
			ResourceHandle<Texture>		blur_output_texture;

			void init(RenderTarget* rt) override;
			void pass(Renderer* renderer) override;

			void set_input_texture(ResourceHandle<Texture> input_texture);
		};

		struct RenderPass_UI : RenderPass
		{
			Profile::FPSGraph ui_fps_graph;

			void init(RenderTarget* rt) override;
			void pass(Renderer* renderer) override;
		};

		struct Light
		{
			Math::v3f position = { 0,0,0 };
			Math::v3f color = { 1,1,1 };
			float power = 1.f;
		};

		struct RenderPass_Lighting : RenderPass
		{

			Light point_lights[MAX_LIGHTS];
			int light_count;

			ResourceHandle<Material>	lighting_material;
			ResourceHandle<Texture>		lighting_ssao_texture;
			ResourceHandle<Texture>		lighting_output_texture;

			void init(RenderTarget* rt) override;
			void pass(Renderer* renderer) override;

			void set_input_texture(ResourceHandle<Texture> input_texture);
		};

		class Renderer
		{
		public:
			Window* window;
			SDL_GLContext gl_context_handle;
			Math::Primitives::u32_rect render_viewport;
			Camera camera;

			// Render passes
			RenderPass_GBuffer		pass_gbuffer;
			RenderPass_SSAO			pass_ssao;
			RenderPass_Blur			pass_blur;
			RenderPass_UI			pass_ui;
			RenderPass_Lighting		pass_lighting;

			void init_window(uint32_t w, uint32_t h, uint32_t x, uint32_t y, const char* title);
			void init_api_context();

			void set_viewport(Math::Primitives::u32_rect viewport_rect);
			void set_camera(float x, float y, float z, float fov, float aspect);

			void init(uint32_t w, uint32_t h, uint32_t x, uint32_t y);
			void destroy();

			void clear(const float& r = 0.0f, const float& g = 0.0f, const float& b = 0.0f, const float& a = 0.0f);
			void render();
			void flush();
		};
	}
}