#include "stdafx.h"
#include "render/rendering_resources.h"
#include "render/renderer.h"
#include <glad/glad.h>
#include "debugging/gl.h"

void Renderer::init(uint32_t w, uint32_t h, uint32_t x, uint32_t y)
{
	// Initializing API
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		SDL_GL_SetSwapInterval(0);	// No vsync
	}

	// Initializing window and context
	CreateNewWindow(w, h, x, y, APP_TITLE);
	CreateNewRenderContext();
	RetrieveAPIFunctionLocations();

	// Initializing render resource managers
	g_bufferResourceManager.init();
	g_textureResourceManager.init();
	g_shaderResourceManager.init();

	// Initializing camera
	float camera_aspect = (float)w / (float)h;
	SetCamera(DEFAULT_CAMERA_X, DEFAULT_CAMERA_Y, DEFAULT_CAMERA_Z, DEFAULT_CAMERA_FOV, camera_aspect);

	// Initializing other state
	GL(glEnable(GL_DEPTH_TEST));
	GL(glEnable(GL_CULL_FACE));
	GL(glFrontFace(GL_CCW));

	setViewport(w, h, 0, 0);
}