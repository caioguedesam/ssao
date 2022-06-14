#include "stdafx.h"
#include "gui/gui.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl3.h"

#include "render/renderer.h"

void GUI::Init(Renderer* renderer)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // TODO_#GUI: Support docking and multiple viewports (look at sdl_gl3 example on imgui, it has what you need)
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(renderer->pWindow->handle, renderer->pGlContextHandle);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void GUI::ProcessSDLEvent(SDL_Event* event)
{
    ImGui_ImplSDL2_ProcessEvent(event);
}

void GUI::Destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void GUI::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void GUI::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::BeginWindow(const char* title, const uint32_t& w, const uint32_t& h, const uint32_t& x, const uint32_t& y)
{
    ImGui::SetNextWindowPos(ImVec2(x, y));
    ImGui::SetNextWindowSize(ImVec2(w, h));
    ImGui::Begin(title);
}

void GUI::EndWindow()
{
    ImGui::End();
}

void GUI::Text(const char* text)
{
    ImGui::Text(text);
}

bool GUI::Button(const char* label)
{
    return ImGui::Button(label);
}

void GUI::Checkbox(const char* label, bool* value)
{
    ImGui::Checkbox(label, value);
}