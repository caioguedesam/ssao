#include "stdafx.h"
#include "resource/buffer_resource_manager.h"
#include "resource/shader_resource_manager.h"
#include "render/renderable.h"
#include <glad/glad.h>
#include "debugging/gl.h"
#include "globals.h"

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

void ImmediateRenderable::init(uint32_t viewportWidth, uint32_t viewportHeight)
{
	// Initialize vertex and index buffers
	uint32_t attrSize = sizeof(ImmediateVertexAttr) / sizeof(float);
	uint32_t vertexCount = MAX_IMMEDIATE_RENDERABLE_TRIS * 3;
	uint32_t indexCount = MAX_IMMEDIATE_RENDERABLE_TRIS * 3;
	g_bufferResourceManager.createBuffer(
		{
			BufferType::VERTEX_BUFFER,
			BufferFormat::R32_FLOAT,
			vertexCount * attrSize
		}, vertexData
	);
	g_bufferResourceManager.createBuffer(
		{
			BufferType::INDEX_BUFFER,
			BufferFormat::R32_UINT,
			indexCount
		}, indexData
	);

	// Initialize default material and shader pipeline
	material = new Material();
	ResourceHandle<Shader> vs = g_shaderResourceManager.getFromFile(SHADERS_PATH"immediate_vs.vert");
	ResourceHandle<Shader> ps = g_shaderResourceManager.getFromFile(SHADERS_PATH"immediate_ps.frag");
	material->init(g_shaderResourceManager.createLinkedShaderPipeline(vs, ps));
	material->shaderPipeline.setUniform("viewportWidth", (int)viewportWidth);
	material->shaderPipeline.setUniform("viewportHeight", (int)viewportHeight);
}

void ImmediateRenderable::addQuad(uint32_t w, uint32_t h, uint32_t x, uint32_t y, float r, float g, float b)
{
	vertexData[vertexCursor++] = { (float)x,		(float)y,			0.f, r, g, b };
	vertexData[vertexCursor++] = { (float)x,		(float)(y + h),		0.f, r, g, b };
	vertexData[vertexCursor++] = { (float)(x + w),	(float)(y + h),		0.f, r, g, b };
	vertexData[vertexCursor++] = { (float)(x + w),	(float)y,			0.f, r, g, b };

	indexData[indexCursor++] = 0;
	indexData[indexCursor++] = 2;
	indexData[indexCursor++] = 1;
	indexData[indexCursor++] = 0;
	indexData[indexCursor++] = 3;
	indexData[indexCursor++] = 2;
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
		GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ImmediateVertexAttr), (void*)0));
		GL(glEnableVertexAttribArray(0));

		GL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ImmediateVertexAttr), (void*)(3 * sizeof(float))));
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

	// Draw
	material->bind();
	// TODO_SHADER: Figure out how to deal with matrix pipeline on immediate renderable
	bindStandardUniforms(material->shaderPipeline, params);
	GL(glBindVertexArray(vaoHandle));
	GL(glDrawElements(GL_TRIANGLES, indexCursor, GL_UNSIGNED_INT, 0));

	// Clear vertex data for next draw
	memset(vertexData, 0, sizeof(vertexData));
	memset(indexData, 0, sizeof(indexData));
	vertexCursor = 0;
	indexCursor = 0;
}