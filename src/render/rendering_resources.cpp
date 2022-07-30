#include "stdafx.h"
#include "render/rendering_resources.h"
#include "globals.h"

float defaultQuadVertexData[] =
{
	// position			// normal			// uvs
	-1.f, -1.f, 0.f,	0.f, 0.f, 0.f,		0.f, 0.f,
	-1.f, 1.f, 0.f,		0.f, 0.f, 0.f,		0.f, 1.f,
	1.f, 1.f, 0.f,		0.f, 0.f, 0.f,		1.f, 1.f,
	1.f, -1.f, 0.f,		0.f, 0.f, 0.f,		1.f, 0.f,
};

uint32_t defaultQuadIndexData[] =
{
	0, 2, 1,
	0, 3, 2,
};

// TODO_RENDER: Default cube data

GBuffer g_gBuffer;

ResourceHandle<Buffer> g_defaultQuadVertexBuffer;
ResourceHandle<Buffer> g_defaultQuadIndexBuffer;

Renderable g_renderableScreenQuad;

RenderTarget g_rtGeometry;
RenderTarget g_rtSSAO;
RenderTarget g_rtSSAOBlur;
RenderTarget g_rtDebugFPS;
RenderTarget g_rtLighting;

void initGlobalRenderResources()
{
	// Default primitives
	g_defaultQuadVertexBuffer = g_bufferResourceManager.createBuffer(
		{
			BufferType::VERTEX_BUFFER,
			BufferFormat::R32_FLOAT,
			sizeof(defaultQuadVertexData) / sizeof(float)
		}, defaultQuadVertexData
	);
	g_defaultQuadIndexBuffer = g_bufferResourceManager.createBuffer(
		{
			BufferType::INDEX_BUFFER,
			BufferFormat::R32_UINT,
			sizeof(defaultQuadIndexData) / sizeof(uint32_t)
		}, defaultQuadIndexData
	);

	// Default renderables
	g_renderableScreenQuad.setVertexData(g_defaultQuadVertexBuffer, g_defaultQuadIndexBuffer);

	// G-Buffer
	g_gBuffer.diffuse = g_textureResourceManager.createTexture({ GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT, TextureFormat::R8_G8_B8_A8_UNORM }, nullptr);
	g_gBuffer.position = g_textureResourceManager.createTexture({ GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT, TextureFormat::R16_G16_B16_A16_FLOAT, TextureParams::WRAP_CLAMP_EDGE }, nullptr);
	g_gBuffer.normal = g_textureResourceManager.createTexture({ GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT, TextureFormat::R16_G16_B16_A16_FLOAT }, nullptr);

	// Render targets
	g_rtGeometry.init(GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT);
	g_rtSSAO.init(GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT);
	g_rtSSAOBlur.init(GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT);
	g_rtDebugFPS.init(DEBUG_FPS_WIDTH, DEBUG_FPS_HEIGHT);
	g_rtLighting.init(GAME_RENDER_WIDTH, GAME_RENDER_HEIGHT);
}