#pragma once
#include "stdafx.h"
#include "resource/buffer_resource_manager.h"
#include "resource/texture_resource_manager.h"
#include "resource/shader_resource_manager.h"
#include "render/render_target.h"
#include "render/renderable.h"
#include "render/camera.h"

struct GBuffer
{
	ResourceHandle<Texture> diffuse;
	ResourceHandle<Texture> position;
	ResourceHandle<Texture> normal;
};
extern GBuffer g_gBuffer;

extern ResourceHandle<Buffer> g_defaultQuadVertexBuffer;
extern ResourceHandle<Buffer> g_defaultQuadIndexBuffer;

extern Renderable g_renderableScreenQuad;

extern RenderTarget g_rtGBuffer;
extern RenderTarget g_rtSSAO;
extern RenderTarget g_rtBlur;
extern RenderTarget g_rtUI;
extern RenderTarget g_rtLighting;

void initGlobalRenderResources();

