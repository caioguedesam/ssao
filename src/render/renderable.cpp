#include "render/renderable.h"
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

void Renderable::Draw(const RenderParams& params)
{
	material->Bind();
	material->shader->SetUniform("uModel", params.model);
	material->shader->SetUniform("uMV", params.view * params.model);
	material->shader->SetUniform("uVP", params.proj * params.view);
	material->shader->SetUniform("uMVP", params.proj * params.view * params.model);
	// TODO: Change this to a better uniform association system later
	material->shader->SetUniform("tex0", 0);
	material->shader->SetUniform("tex1", 1);
	material->shader->SetUniform("tex2", 2);

	GL(glBindVertexArray(vaoHandle));

	GL(glDrawElements(GL_TRIANGLES, indexBuffer->count, GL_UNSIGNED_INT, 0));
}