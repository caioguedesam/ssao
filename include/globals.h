#pragma once
#include "stdafx.h"

// TODO_RENDER: Move a lot of these guys to some sort of settings file

#define APP_TITLE "Typheus"

#define APP_DEFAULT_WIDTH 1600
#define APP_DEFAULT_HEIGHT 900

#define GAME_RENDER_WIDTH 1920
#define GAME_RENDER_HEIGHT 1080

#define MAIN_WINDOW_DEFAULT_WIDTH 1280
#define MAIN_WINDOW_DEFAULT_HEIGHT 720

#define DEBUG_FPS_WIDTH 800
#define DEBUG_FPS_HEIGHT 200

#define DEFAULT_CAMERA_X 0
#define DEFAULT_CAMERA_Y 0
#define DEFAULT_CAMERA_Z 0
#define DEFAULT_CAMERA_FOV 45

#define RESOURCES_PATH "./resources/"
#define SHADERS_PATH RESOURCES_PATH"shaders/"
#define MODELS_PATH RESOURCES_PATH"models/"
#define TEXTURES_PATH RESOURCES_PATH"textures/"

extern uint32_t g_screenWidth;
extern uint32_t g_screenHeight;