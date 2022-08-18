#pragma once
#include "stdafx.h"

#include "render/buffer.h"
#include "render/material.h"

#define MAX_IMMEDIATE_RENDERABLE_TRIS 2048

namespace Ty
{
	namespace Graphics
	{
		struct RenderParams
		{
			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 proj;
		};

		struct Renderable
		{
			uint32_t vaoHandle = HANDLE_INVALID;

			ResourceHandle<Buffer> vertexBuffer;
			ResourceHandle<Buffer> indexBuffer;

			Material* material;
			glm::mat4 uModel;

			virtual void setVertexData(ResourceHandle<Buffer> vertexBuffer, ResourceHandle<Buffer> indexBuffer);
			void setMaterial(Material* mat);

			virtual void draw(const RenderParams& params);
		};

		struct ImmediateVertexAttr
		{
			float x, y, z;
			float r, g, b, a;
		};

		struct ImmediateRenderable : Renderable		// TODO_RENDER: Use GPU instancing here to save memory and performance
		{
			ImmediateVertexAttr vertexData[MAX_IMMEDIATE_RENDERABLE_TRIS * 3];
			uint32_t vertexCursor = 0;
			uint32_t indexData[MAX_IMMEDIATE_RENDERABLE_TRIS * 3];
			uint32_t indexCursor = 0;

			uint32_t viewportWidth = 0;
			uint32_t viewportHeight = 0;

			void init(uint32_t viewportWidth, uint32_t viewportHeight);
			void clear();
			void addQuad(float w, float h, float x, float y, float r = 1.f, float g = 1.f, float b = 1.f, float a = 1.f);
			// TODO_RENDER: Add more shapes later

			void setVertexData(ResourceHandle<Buffer> vertexBuffer, ResourceHandle<Buffer> indexBuffer) override;
			void draw(const RenderParams& params) override;
		};

	}
}