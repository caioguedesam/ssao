#include "stdafx.h"
#include "resource/buffer_resource_manager.h"
#include "resource/shader_resource_manager.h"
#include "render/renderable.h"
#include <glad/glad.h>
#include "debugging/gl.h"

void Renderable::setVertexData(ResourceHandle<Buffer> vertexBuffer, ResourceHandle<Buffer> indexBuffer)
{
	if (vaoHandle == HANDLE_INVALID)
	{
		GL(glGenVertexArrays(1, &vaoHandle));
	}

	this->vertexBuffer = vertexBuffer;
	this->indexBuffer = indexBuffer;

	GL(glBindVertexArray(vaoHandle));

	g_bufferResourceManager.bindBuffer(vertexBuffer);
	g_bufferResourceManager.bindBuffer(indexBuffer);

	{
		// For vertex buffers: vertex position (x, y, z) -> vertex normals (x, y, z) -> vertex UVs (u, v)
		GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * g_bufferResourceManager.get(vertexBuffer)->getStride(), (void*)0));
		GL(glEnableVertexAttribArray(0));

		GL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * g_bufferResourceManager.get(vertexBuffer)->getStride(), (void*)(3 * sizeof(float))));
		GL(glEnableVertexAttribArray(1));

		GL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * g_bufferResourceManager.get(vertexBuffer)->getStride(), (void*)(6 * sizeof(float))));
		GL(glEnableVertexAttribArray(2));
	}

	GL(glBindVertexArray(0));
}

void Renderable::setMaterial(Material* mat)
{
	material = mat;
}

void bindStandardUniforms(ShaderPipeline shaderPipeline, const RenderParams& params)
{
	// TODO_SHADER: Change "model" to "world" matrix. WVP is just a better name.
	shaderPipeline.setUniform("uModel", params.model);
	shaderPipeline.setUniform("uView", params.view);
	shaderPipeline.setUniform("uProj", params.proj);
	shaderPipeline.setUniform("uMV", params.view * params.model);
	shaderPipeline.setUniform("uVP", params.proj * params.view);
	shaderPipeline.setUniform("uMVP", params.proj * params.view * params.model);

	// TODO_SHADER: Get per uniform names from shader instead of this.
	shaderPipeline.setUniform("tex0", 0);
	shaderPipeline.setUniform("tex1", 1);
	shaderPipeline.setUniform("tex2", 2);
	shaderPipeline.setUniform("tex3", 3);
}

void Renderable::draw(const RenderParams& params)
{
	material->bind();

	// TODO_SHADER: Make a better process for binding all this default stuff, uniform buffers, etc.
	// I like OGLDev's pipeline stuff.

	bindStandardUniforms(material->shaderPipeline, params);

	GL(glBindVertexArray(vaoHandle));

	GL(glDrawElements(GL_TRIANGLES, g_bufferResourceManager.get(indexBuffer)->getCount(), GL_UNSIGNED_INT, 0));
}

void ImmediateRenderable::addQuad(uint32_t w, uint32_t h, uint32_t x, uint32_t y, float r, float g, float b)
{
	// Vertex 1
	vertexData[vertexCursor].x = x;
	vertexData[vertexCursor].y = y;
	vertexData[vertexCursor].z = 0.f;
	vertexData[vertexCursor].r = r;
	vertexData[vertexCursor].g = g;
	vertexData[vertexCursor].b = b;
	vertexCursor++;
}

void ImmediateRenderable::setVertexData(ResourceHandle<Buffer> vertexBuffer, ResourceHandle<Buffer> indexBuffer)
{
	if (vaoHandle == HANDLE_INVALID)
	{
		GL(glGenVertexArrays(1, &vaoHandle));
	}

	this->vertexBuffer = vertexBuffer;
	this->indexBuffer = indexBuffer;

	GL(glBindVertexArray(vaoHandle));

	g_bufferResourceManager.bindBuffer(vertexBuffer);
	g_bufferResourceManager.bindBuffer(indexBuffer);

	{
		// For vertex buffers (immediate): vertex position (x, y, z) -> vertex colors (r, g, b)
		GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * g_bufferResourceManager.get(vertexBuffer)->getStride(), (void*)0));
		GL(glEnableVertexAttribArray(0));

		GL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * g_bufferResourceManager.get(vertexBuffer)->getStride(), (void*)(3 * sizeof(float))));
		GL(glEnableVertexAttribArray(1));
	}

	GL(glBindVertexArray(0));
}

void ImmediateRenderable::draw(const RenderParams& params)
{
	// TODO_RENDER: These have to be created on renderable init, with proper size
	g_bufferResourceManager.setBufferData(vertexBuffer, vertexData);
	g_bufferResourceManager.setBufferData(indexBuffer, indexData);
	setVertexData(vertexBuffer, indexBuffer);

	Renderable::draw(params);

	// Clear vertex data for next draw
	memset(vertexData, 0, sizeof(vertexData));
	memset(indexData, 0, sizeof(indexData));
	vertexCursor = 0;
	indexCursor = 0;
}