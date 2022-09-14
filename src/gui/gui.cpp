#include "stdafx.h"
#include "gui/gui.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl3.h"
#include "app.h"
#include "time/time.h"

namespace Ty
{
    namespace UI
    {
        GUIState g_gui_state;

        void GUI::init(App* app)
        {
            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            // TODO_GUI: Support docking and multiple viewports (look at sdl_gl3 example on imgui, it has what you need)
            //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
            //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

            ImGuiStyle& style = ImGui::GetStyle();
            style.WindowPadding = ImVec2(0.0f, 0.0f);
            style.WindowBorderSize = 0.f;
            style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.50f);

            // Setup Platform/Renderer backends
            SDL_Window* sdl_window = app->renderer.window->handle;
            SDL_GLContext sdl_context = app->renderer.gl_context_handle;
            ImGui_ImplSDL2_InitForOpenGL(sdl_window, sdl_context);
            ImGui_ImplOpenGL3_Init("#version 330");
        }

        void GUI::process_event(SDL_Event* event)
        {
            ImGui_ImplSDL2_ProcessEvent(event);

            if (event->type == SDL_KEYDOWN)
            {
                SDL_Keysym key = event->key.keysym;
                if (key.sym == SDLK_f)
                {
                    g_gui_state.show_fps_graph = !g_gui_state.show_fps_graph;     // TODO_INPUT, TODO_RENDER: This should trigger something in Renderer to disable fps graph rendering. Also should use remade input system.
                }
            }
        }

        void GUI::destroy()
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplSDL2_Shutdown();
            ImGui::DestroyContext();
        }

        void GUI::begin_frame()
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();
        }

        void GUI::end_frame()
        {
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        void GUI::begin_window(const char* title, const uint32_t& w, const uint32_t& h, const uint32_t& x, const uint32_t& y, ImGuiWindowFlags windowFlags)
        {
            ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Once);      // TODO_GUI: Calling this continuously makes windows always reset size and position, so support for moving/resizing windows is broken.
            ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_Once);
            ImGui::Begin(title, 0, windowFlags);
        }

        void GUI::end_window()
        {
            ImGui::End();
        }

        void GUI::image(Graphics::ResourceHandle<Graphics::Texture> textureHandle, uint32_t w, uint32_t h)
        {
            ImGui::Image(
                (void*)(intptr_t)Graphics::texture_resource_manager.get(textureHandle)->api_handle,
                ImVec2(w, h),
                ImVec2(0, 1), ImVec2(1, 0)  // OpenGL maps textures down to up left to right, so invert v coordinate.
            );
        }

        void GUI::display(App* app)
        {
            begin_frame();

            Graphics::Renderer& renderer = app->renderer;
            // Main game window
            {
                uint32_t w = MAIN_WINDOW_DEFAULT_WIDTH;
                uint32_t h = MAIN_WINDOW_DEFAULT_HEIGHT;
                begin_window("Game", w, h, APP_DEFAULT_WIDTH / 2 - w / 2, APP_DEFAULT_HEIGHT / 2 - h / 2, ImGuiWindowFlags_NoTitleBar);
                image(renderer.pass_lighting.lighting_output_texture, w, h);
                end_window();
            }

            // Sidebar
            if(g_gui_state.show_sidebar)
            {
                begin_window("Properties", 320, APP_DEFAULT_HEIGHT, 0, 0);   // TODO_RESIZE, TODO_GUI: Change this to resize when window changes size

                ImGui::Text("FPS: %.1lf", Time::get_fps());
                ImGui::Checkbox("Blur pass", &renderer.pass_blur.enabled);

                Graphics::RenderPass_SSAO& pass_ssao = renderer.pass_ssao;
                struct ssao_parameters
                {
                    int kernelSize;
                    int noiseDimension;
                    float radius;
                };
                static ssao_parameters lass_ssao_params;
                lass_ssao_params =
                {
                    pass_ssao.ssao_data.sample_amount,
                    pass_ssao.ssao_data.white_noise_dimension,
                    pass_ssao.ssao_data.sample_radius
                };
                ssao_parameters new_ssao_params = lass_ssao_params;
                ImGui::SliderInt("Kernel size", &new_ssao_params.kernelSize, 0, MAX_SSAO_KERNEL_SIZE);
                if (ImGui::IsItemHovered())
                {
                    ImGui::SetTooltip("How many points will be pseudo-randomly sampled for each screen pixel in the SSAO pass.");
                }
                ImGui::SliderInt("Noise dimension", &new_ssao_params.noiseDimension, 0, MAX_SSAO_NOISE_DIMENSION);
                if (ImGui::IsItemHovered())
                {
                    ImGui::SetTooltip("The dimension of the square noise texture used for pseudo-random kernel rotations in the SSAO pass.");
                }
                ImGui::SliderFloat("Radius", &new_ssao_params.radius, 0, MAX_SSAO_RADIUS);
                if (ImGui::IsItemHovered())
                {
                    ImGui::SetTooltip("The maximum radius that the samples will be projected to, in view space.");
                }
                if (lass_ssao_params.kernelSize != new_ssao_params.kernelSize)
                {
                    pass_ssao.ssao_data.sample_amount = new_ssao_params.kernelSize;
                    pass_ssao.generate_sample_kernel();
                    pass_ssao.bind_sample_kernel();
                }
                if (lass_ssao_params.noiseDimension != new_ssao_params.noiseDimension)
                {
                    pass_ssao.ssao_data.white_noise_dimension = new_ssao_params.noiseDimension;
                    pass_ssao.generate_white_noise();
                    pass_ssao.bind_white_noise_texture();
                }
                if (lass_ssao_params.radius != new_ssao_params.radius)
                {
                    pass_ssao.ssao_data.sample_radius = new_ssao_params.radius;
                    pass_ssao.bind_sample_radius();
                }
                lass_ssao_params = new_ssao_params;

                end_window();
            }

            // FPS graph
            if (g_gui_state.show_fps_graph)
            {
                uint32_t w = FPS_WINDOW_WIDTH;
                uint32_t h = FPS_WINDOW_HEIGHT;
                begin_window("FPS", w, h, APP_DEFAULT_WIDTH - w, APP_DEFAULT_HEIGHT - h, ImGuiWindowFlags_NoTitleBar);
                image(renderer.pass_ui.ui_fps_graph.fps_graph_texture, w, h);
                end_window();
            }

            end_frame();
        }
    }
}