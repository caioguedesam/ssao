#pragma once
#include "stdafx.h"

#include "render/buffer.h"
#include "render/material.h"

struct RenderParams
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

class Renderable
{
public:
	uint32_t vaoHandle = UINT32_MAX;
	Buffer* vertexBuffer;
	Buffer* indexBuffer;
	//Shader* shader;
	Material* material;
	glm::mat4 uModel;

	void SetVertexData(Buffer* vb, Buffer* ib);

	//void SetShader(Shader* sh);
	void SetMaterial(Material* mat);

	void Draw(const RenderParams& params);
};
