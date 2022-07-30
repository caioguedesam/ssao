#pragma once
#include "stdafx.h"
#include "imgui.h"
#include <SDL.h>
#include "resource/texture_resource_manager.h"

class App;

struct GUIState
{
	bool show_fps_graph = false;
};

extern GUIState g_guiState;

class GUI
{
public:
	static void init(App* app);
	static void processEvent(SDL_Event* event);
	static void display(App* app);
	static void destroy();

private:
	static void beginFrame();
	static void endFrame();
	static void beginWindow(const char* title, const uint32_t& w, const uint32_t& h, const uint32_t& x, const uint32_t& y, ImGuiWindowFlags windowFlags = 0);
	static void endWindow();
	static void image(ResourceHandle<Texture> textureHandle, uint32_t w, uint32_t h);
};
