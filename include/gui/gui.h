#pragma once
#include "stdafx.h"
#include "imgui.h"
#include <SDL.h>
#include "resource/texture_resource_manager.h"

namespace Ty
{
	class App;

	namespace UI
	{
		struct GUIState
		{
			bool show_fps_graph = false;
			bool show_sidebar = false;
		};

		extern GUIState g_gui_state;

		class GUI
		{
		public:
			static void init(App* app);
			static void process_event(SDL_Event* event);
			static void display(App* app);
			static void destroy();

		private:
			static void begin_frame();
			static void end_frame();
			static void begin_window(const char* title, const uint32_t& w, const uint32_t& h, const uint32_t& x, const uint32_t& y, ImGuiWindowFlags window_flags = 0);
			static void end_window();
			static void image(Graphics::ResourceHandle<Graphics::Texture> texture_handle, uint32_t w, uint32_t h);
		};
	}
}
