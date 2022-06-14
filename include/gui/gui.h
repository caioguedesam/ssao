#pragma once
#include "stdafx.h"
#include "imgui.h"

class Renderer;

class GUI
{
public:

	static void Init(Renderer* renderer);
	static void Destroy();
	static void BeginFrame();
	static void EndFrame();

	static void BeginWindow(const char* title, const uint32_t& w, const uint32_t& h, const uint32_t& x, const uint32_t& y);
	static void EndWindow();
	static void Text(const char* text);
	static bool Button(const char* label);
};
