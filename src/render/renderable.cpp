#include "stdafx.h"
#include "resource/shader_resource_manager.h"
#include "render/renderable.h"
#include <glad/glad.h>
#include "debugging/gl.h"

void Renderable::SetVertexData(Buffer* vb, Buffer* ib)
{
	if (vaoHandle == UINT32_MAX)
	{
		GL(glGenVertexArrays(1, &vaoHandle));
	}

	vertexBuffer = vb;
	indexBuffer = ib;

	GL(glBindVertexArray(vaoHandle));
	vertexBuffer->Bind(GL_ARRAY_BUFFER);
	indexBuffer->Bind(GL_ELEMENT_ARRAY_BUFFER);

	GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * vertexBuffer->sizeInBytes / vertexBuffer->count, (void*)0));
	GL(glEnableVertexAttribArray(0));

	GL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * vertexBuffer->sizeInBytes / vertexBuffer->count, (void*)(3 * sizeof(float))));
	GL(glEnableVertexAttribArray(1));

	GL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * vertexBuffer->sizeInBytes / vertexBuffer->count, (void*)(6 * sizeof(float))));
	GL(glEnableVertexAttribArray(2));

	GL(glBindVertexArray(0));
}

void Renderable::SetMaterial(Material* mat)
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

void Renderable::Draw(const RenderParams& params)
{
	material->bind();

	// TODO_SHADER: Make a better process for binding all this default stuff, uniform buffers, etc.
	// I like OGLDev's pipeline stuff.

	bindStandardUniforms(material->shaderPipeline, params);

	GL(glBindVertexArray(vaoHandle));

	GL(glDrawElements(GL_TRIANGLES, indexBuffer->count, GL_UNSIGNED_INT, 0));
}