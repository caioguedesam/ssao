#include "render/renderable.h"

void Renderable::SetVertexData(Buffer* vb, Buffer* ib)
{
	if (vaoHandle == UINT32_MAX)
	{
		glGenVertexArrays(1, &vaoHandle);
		ASSERT(glGetError() == GL_NO_ERROR, "");
	}

	vertexBuffer = vb;
	indexBuffer = ib;

	glBindVertexArray(vaoHandle);
	ASSERT(glGetError() == GL_NO_ERROR, "");
	vertexBuffer->Bind(GL_ARRAY_BUFFER);
	indexBuffer->Bind(GL_ELEMENT_ARRAY_BUFFER);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * vertexBuffer->sizeInBytes / vertexBuffer->count, (void*)0);
	ASSERT(glGetError() == GL_NO_ERROR, "");
	glEnableVertexAttribArray(0);
	ASSERT(glGetError() == GL_NO_ERROR, "");

	// TODO: Start from here, textures
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * vertexBuffer->sizeInBytes / vertexBuffer->count, (void*)(3 * sizeof(float)));
	ASSERT(glGetError() == GL_NO_ERROR, "");
	glEnableVertexAttribArray(1);
	ASSERT(glGetError() == GL_NO_ERROR, "");

	glBindVertexArray(0);
	ASSERT(glGetError() == GL_NO_ERROR, "");
}

void Renderable::SetShader(Shader* sh)
{
	shader = sh;
}

void Renderable::Draw(const RenderParams& params)
{
	shader->Bind();
	shader->SetUniform("uModel", params.model);
	shader->SetUniform("uVP", params.proj * params.view);
	shader->SetUniform("uMVP", params.proj * params.view * params.model);

	glBindVertexArray(vaoHandle);
	ASSERT(glGetError() == GL_NO_ERROR, "Couldn't bind vertex array.");

	glDrawElements(GL_TRIANGLES, indexBuffer->count, GL_UNSIGNED_INT, 0);
	ASSERT(glGetError() == GL_NO_ERROR, "Failed drawing renderable.");
}