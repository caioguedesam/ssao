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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * vertexBuffer->sizeInBytes / vertexBuffer->count, (void*)0);
	ASSERT(glGetError() == GL_NO_ERROR, "");
	glEnableVertexAttribArray(0);
	ASSERT(glGetError() == GL_NO_ERROR, "");

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * vertexBuffer->sizeInBytes / vertexBuffer->count, (void*)(3 * sizeof(float)));
	ASSERT(glGetError() == GL_NO_ERROR, "");
	glEnableVertexAttribArray(1);
	ASSERT(glGetError() == GL_NO_ERROR, "");

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * vertexBuffer->sizeInBytes / vertexBuffer->count, (void*)(6 * sizeof(float)));
	ASSERT(glGetError() == GL_NO_ERROR, "");
	glEnableVertexAttribArray(2);
	ASSERT(glGetError() == GL_NO_ERROR, "");

	glBindVertexArray(0);
	ASSERT(glGetError() == GL_NO_ERROR, "");
}

//void Renderable::SetShader(Shader* sh)
//{
//	shader = sh;
//}

void Renderable::SetMaterial(Material* mat)
{
	material = mat;
}

void Renderable::Draw(const RenderParams& params)
{
	material->Bind();
	material->shader->SetUniform("uModel", params.model);
	material->shader->SetUniform("uVP", params.proj * params.view);
	material->shader->SetUniform("uMVP", params.proj * params.view * params.model);

	glBindVertexArray(vaoHandle);
	ASSERT(glGetError() == GL_NO_ERROR, "Couldn't bind vertex array.");

	glDrawElements(GL_TRIANGLES, indexBuffer->count, GL_UNSIGNED_INT, 0);
	ASSERT(glGetError() == GL_NO_ERROR, "Failed drawing renderable.");
}