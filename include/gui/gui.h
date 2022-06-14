#pragma once
#include "stdafx.h"
#include "imgui.h"
#include <SDL.h>

class Renderer;

class GUI
{
public:

	static void Init(Renderer* renderer);
	static void ProcessSDLEvent(SDL_Event* event);
	static void Destroy();
	static void BeginFrame();
	static void EndFrame();

	static void BeginWindow(const char* title, const uint32_t& w, const uint32_t& h, const uint32_t& x, const uint32_t& y);
	static void EndWindow();
	static void Text(const char* text);
	static bool Button(const char* label);
	static void Checkbox(const char* label, bool* value);
	static void Slider_int(const char* label, int* value, const uint32_t& minValue, const uint32_t& maxValue);
};
