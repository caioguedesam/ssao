#pragma once
#include "stdafx.h"
#include "imgui.h"
#include "resource/texture_resource_manager.h"
#include <SDL.h>

class Renderer;

class GUI
{
public:

	static void init(Renderer* renderer);
	static void processSDLEvent(SDL_Event* event);
	static void destroy();
	static void beginFrame();
	static void endFrame();

	static void beginWindow(const char* title, const uint32_t& w, const uint32_t& h, const uint32_t& x, const uint32_t& y);
	static void endWindow();
	static void text(const char* text);
	static bool button(const char* label);
	static void checkbox(const char* label, bool* value);
	static void slider_int(const char* label, int* value, const uint32_t& minValue, const uint32_t& maxValue);
	static void slider_float(const char* label, float* value, const float& minValue, const float& maxValue);
	static void image(ResourceHandle<Texture> textureHandle, uint32_t w, uint32_t h);
};
