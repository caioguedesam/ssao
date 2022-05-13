#pragma once
#include <stdint.h>

#define APP_DEFAULT_WIDTH 640
#define APP_DEFAULT_HEIGHT 480

#define RESOURCES_PATH "./resources/"
#define SHADERS_PATH RESOURCES_PATH"shaders/"
#define MODELS_PATH RESOURCES_PATH"models/"
#define TEXTURES_PATH RESOURCES_PATH"textures/"

extern uint32_t g_screenWidth;
extern uint32_t g_screenHeight;