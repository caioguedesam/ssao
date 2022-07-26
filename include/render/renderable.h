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
	uint32_t vaoHandle = HANDLE_INVALID;
	
	ResourceHandle<Buffer> vertexBuffer;
	ResourceHandle<Buffer> indexBuffer;

	Material* material;
	glm::mat4 uModel;

	void setVertexData(ResourceHandle<Buffer> vertexBuffer, ResourceHandle<Buffer> indexBuffer, bool onlyPositions = false);
	void setMaterial(Material* mat);

	void draw(const RenderParams& params);
};
