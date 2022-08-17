#include "stdafx.h"
#include "gui/gui.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl3.h"
#include "app.h"
#include "time/time.h"

GUIState g_guiState;

void GUI::init(App* app)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // TODO_GUI: Support docking and multiple viewports (look at sdl_gl3 example on imgui, it has what you need)
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    //ImGui::StyleColorsDark();

    // Applying ImGui style
    // https://github.com/ocornut/imgui/issues/707
    io.Fonts->AddFontFromFileTTF(".\\dependencies\\imgui\\misc\\fonts\\Roboto-Medium.ttf", 14);
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
    colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(0.00f, 0.00f);
    style.FramePadding = ImVec2(5.00f, 2.00f);
    style.CellPadding = ImVec2(6.00f, 6.00f);
    style.ItemSpacing = ImVec2(6.00f, 6.00f);
    style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
    style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
    style.IndentSpacing = 25;
    style.ScrollbarSize = 5;
    style.GrabMinSize = 10;
    style.WindowBorderSize = 1;
    style.ChildBorderSize = 1;
    style.PopupBorderSize = 1;
    style.FrameBorderSize = 1;
    style.TabBorderSize = 1;
    style.TabBorderSize = 0;
    style.WindowRounding = 0;
    style.ChildRounding = 0;
    style.FrameRounding = 0;
    style.PopupRounding = 0;
    style.ScrollbarRounding = 9;
    style.GrabRounding = 3;
    style.LogSliderDeadzone = 4;
    style.TabRounding = 0;

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    SDL_Window* sdl_window = app->renderer.pWindow->handle;
    SDL_GLContext sdl_context = app->renderer.pGlContextHandle;
    ImGui_ImplSDL2_InitForOpenGL(sdl_window, sdl_context);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void GUI::processEvent(SDL_Event* event)
{
    ImGui_ImplSDL2_ProcessEvent(event);

    if (event->type == SDL_KEYDOWN)
    {
        SDL_Keysym key = event->key.keysym;
        if (key.sym == SDLK_f)
        {
            g_guiState.show_fps_graph = !g_guiState.show_fps_graph;     // TODO_INPUT, TODO_RENDER: This should trigger something in Renderer to disable fps graph rendering. Also should use remade input system.
        }
    }
}

void GUI::destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void GUI::beginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void GUI::endFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::beginWindow(const char* title, const uint32_t& w, const uint32_t& h, const uint32_t& x, const uint32_t& y, ImGuiWindowFlags windowFlags)
{
    ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Once);      // TODO_GUI: Calling this continuously makes windows always reset size and position, so support for moving/resizing windows is broken.
    ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_Once);
    ImGui::Begin(title, 0, windowFlags);
}

void GUI::endWindow()
{
    ImGui::End();
}

void GUI::image(ResourceHandle<Texture> textureHandle, uint32_t w, uint32_t h)
{
    ImGui::Image(
        (void*)(intptr_t)g_textureResourceManager.get(textureHandle)->apiHandle,
        ImVec2(w, h),
        ImVec2(0, 1), ImVec2(1, 0)  // OpenGL maps textures down to up left to right, so invert v coordinate.
    );
}

void GUI::display(App* app)
{
    beginFrame();

    Renderer& renderer = app->renderer;
    // Sidebar
    {
        beginWindow("Properties", 320, APP_DEFAULT_HEIGHT, 0, 0);   // TODO_RESIZE, TODO_GUI: Change this to resize when window changes size
        
        ImGui::Text("FPS: %.1lf", Time::fps());
        ImGui::Checkbox("Blur pass", &renderer.pass_blur.enabled);

        RenderPass_SSAO& pass_ssao = renderer.pass_ssao;
        struct ssao_parameters
        {
            int kernelSize;
            int noiseDimension;
            float radius;
        };
        static ssao_parameters lastSsaoParams;
        lastSsaoParams =
        {
            pass_ssao.ssao_data.ssaoKernelSize,
            pass_ssao.ssao_data.ssaoNoiseDimension,
            pass_ssao.ssao_data.ssaoRadius
        };
        ssao_parameters newSsaoParams = lastSsaoParams;
        ImGui::SliderInt("Kernel size", &newSsaoParams.kernelSize, 0, MAX_SSAO_KERNEL_SIZE);
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("How many points will be pseudo-randomly sampled for each screen pixel in the SSAO pass.");
        }
        ImGui::SliderInt("Noise dimension", &newSsaoParams.noiseDimension, 0, MAX_SSAO_NOISE_DIMENSION);
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("The dimension of the square noise texture used for pseudo-random kernel rotations in the SSAO pass.");
        }
        ImGui::SliderFloat("Radius", &newSsaoParams.radius, 0, MAX_SSAO_RADIUS);
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("The maximum radius that the samples will be projected to, in view space.");
        }
        if (lastSsaoParams.kernelSize != newSsaoParams.kernelSize)
        {
            pass_ssao.ssao_data.ssaoKernelSize = newSsaoParams.kernelSize;
            pass_ssao.generateKernel();
            pass_ssao.bindKernel();
        }
        if (lastSsaoParams.noiseDimension != newSsaoParams.noiseDimension)
        {
            pass_ssao.ssao_data.ssaoNoiseDimension = newSsaoParams.noiseDimension;
            pass_ssao.generateNoise();
            pass_ssao.bindNoiseTexture();
        }
        if (lastSsaoParams.radius != newSsaoParams.radius)
        {
            pass_ssao.ssao_data.ssaoRadius = newSsaoParams.radius;
            pass_ssao.bindRadius();
        }
        lastSsaoParams = newSsaoParams;

        endWindow();
    }

    // Main game window
    {
		uint32_t w = MAIN_WINDOW_DEFAULT_WIDTH;
		uint32_t h = MAIN_WINDOW_DEFAULT_HEIGHT;
        beginWindow("Game", w, h, APP_DEFAULT_WIDTH / 2 - w / 2, APP_DEFAULT_HEIGHT / 2 - h / 2, ImGuiWindowFlags_NoTitleBar);
        image(renderer.pass_lighting.lighting_outputTexture, w, h);
		endWindow();
    }

    // FPS graph
    if(g_guiState.show_fps_graph)
    {
		uint32_t w = FPS_WINDOW_WIDTH;
		uint32_t h = FPS_WINDOW_HEIGHT;
		beginWindow("FPS", w, h, APP_DEFAULT_WIDTH - w, APP_DEFAULT_HEIGHT - h, ImGuiWindowFlags_NoTitleBar);
		image(renderer.pass_ui.ui_fpsGraph.fpsGraphTexture, w, h);
		endWindow();
    }

    endFrame();
}