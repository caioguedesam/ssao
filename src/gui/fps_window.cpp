#include "stdafx.h"
#include "globals.h"
#include "gui/fps_window.h"
#include "resource/buffer_resource_manager.h"
#include "resource/shader_resource_manager.h"

//uint32_t quadIndices[] =
//{
//	0, 2, 1,
//	0, 3, 2,
//};

void FPSGraph::init()
{
	memset(framesTracked, 0, sizeof(framesTracked));
	memset(quadVertices, 0, sizeof(quadVertices));
	memset(quadIndices, 0, sizeof(quadIndices));
	memset(frameColors, 0, sizeof(frameColors));

	// Build index array
	uint32_t cursor = 0;
	for (uint32_t f = 0; f < FRAMES_TO_TRACK; f++)
	{
		int i = f * 4;
		// Upper tri
		quadIndices[cursor++]		= i;
		quadIndices[cursor++]		= i + 2;
		quadIndices[cursor++]		= i + 1;

		// Lower tri
		quadIndices[cursor++]		= i;
		quadIndices[cursor++]		= i + 3;
		quadIndices[cursor++]		= i + 2;
	}

	// Initial x coordinate offset
	for (uint32_t f = 0; f < FRAMES_TO_TRACK; f++)
	{
		int i = f * 12;
		quadVertices[i]			= ((double)f / FRAMES_TO_TRACK) * 2 - 1;
		quadVertices[i + 3]		= ((double)f / FRAMES_TO_TRACK) * 2 - 1;
		quadVertices[i + 6]		= ((double)(f + 1) / FRAMES_TO_TRACK) * 2 - 1;
		quadVertices[i + 9]		= ((double)(f + 1) / FRAMES_TO_TRACK) * 2 - 1;
	}
	//// Initial y coordinate offset
	//for (uint32_t f = 0; f < FRAMES_TO_TRACK; f++)
	//{
	//	quadVertices[f * 3 + 1] = -1.;
	//}

	// Setting up graphics resources
	fpsGraphVertexBuffer = g_bufferResourceManager.createBuffer({
			BufferType::VERTEX_BUFFER,
			BufferFormat::R32_FLOAT,
			FRAMES_TO_TRACK * 4 * 3,
		}, quadVertices);
	fpsGraphIndexBuffer = g_bufferResourceManager.createBuffer({
			BufferType::INDEX_BUFFER,
			BufferFormat::R32_UINT,
			FRAMES_TO_TRACK * 6,
		}, quadIndices);

	fpsGraphTexture = g_textureResourceManager.createTexture(
		{
			FPS_WINDOW_WIDTH,
			FPS_WINDOW_HEIGHT,
			TextureFormat::R8_G8_B8_A8_UNORM,
			TextureParams::TEXPARAMS_NONE
		}, nullptr);

	ResourceHandle<Shader> fpsGraphVS = g_shaderResourceManager.getFromFile(SHADERS_PATH"fps_graph_vs.vert");
	ResourceHandle<Shader> fpsGraphPS = g_shaderResourceManager.getFromFile(SHADERS_PATH"fps_graph_ps.frag");
	ShaderPipeline fpsGraphShaderPipeline = g_shaderResourceManager.createLinkedShaderPipeline(fpsGraphVS, fpsGraphPS);
	fpsGraphMaterial.init(fpsGraphShaderPipeline);

	fpsGraphRenderable.setVertexData(fpsGraphVertexBuffer, fpsGraphIndexBuffer, true);
	fpsGraphRenderable.setMaterial(&fpsGraphMaterial);
}

void FPSGraph::setFrameData(double* frameData, int frameCursor)
{
	for (int i = 0; i < FRAMES_TO_TRACK; i++)
	{
		framesTracked[i] = frameData[i];
	}
	this->frameCursor = frameCursor;
}

void FPSGraph::update()
{
	// Get max frame time
	double top = MIN_TOP_FRAME_TIME;
	for (int i = 0; i < FRAMES_TO_TRACK; i++)
	{
		if (framesTracked[i] > top) top = framesTracked[i];
	}

	// Change vertex data
	for (uint32_t f = 0; f < FRAMES_TO_TRACK; f++)
	{
		double frametime = framesTracked[f];
		float relativeFrametime = static_cast<float>(frametime / top);
		// Offset vertex y coordinates
		int i = f * 12;
		quadVertices[i + 4] = relativeFrametime;
		quadVertices[i + 7] = relativeFrametime;
	}

	// Update color buffer
	frameColors[frameCursor * 3]		= 0.;
	frameColors[frameCursor * 3 + 1]	= 1.;
	frameColors[frameCursor * 3 + 2]	= 0.;
	for (uint32_t f = 0; f < FRAMES_TO_TRACK; f++)
	{
		int i = f * 3;
		frameColors[i + 1] -= 0.005 * frameColors[i + 1];
		if (frameColors[i + 1] < 0) frameColors[i + 1] = 0.f;
	}

	g_bufferResourceManager.setBufferData(fpsGraphVertexBuffer, quadVertices);
	fpsGraphRenderable.setVertexData(fpsGraphVertexBuffer, fpsGraphIndexBuffer, true);
}