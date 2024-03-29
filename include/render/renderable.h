#pragma once
#include "stdafx.h"

#include "core/math.h"
#include "render/buffer.h"
#include "render/material.h"
#include "render/model.h"

#define MAX_IMMEDIATE_RENDERABLE_TRIS 2048

namespace Ty
{
	namespace Graphics
	{
		struct RenderParams
		{
			Math::m4f model;
			Math::m4f view;
			Math::m4f proj;
		};

		struct Renderable
		{
			uint32_t vao_api_handle = HANDLE_INVALID;

			ResourceHandle<Buffer> vertex_buffer;
			ResourceHandle<Buffer> index_buffer;

			ResourceHandle<Material> material;
			Math::m4f u_model;

			virtual void set_vertex_data(ResourceHandle<Buffer> vertex_buffer, ResourceHandle<Buffer> index_buffer);
			void set_material(ResourceHandle<Material> mat);

			virtual void draw(const RenderParams& params);
		};

		struct ImmediateVertexAttr
		{
			float x, y, z;
			float r, g, b, a;
		};

		struct ImmediateRenderable : Renderable		// TODO_RENDER: Use GPU instancing here to save memory and performance
		{
			ImmediateVertexAttr vertex_data[MAX_IMMEDIATE_RENDERABLE_TRIS * 3];
			uint32_t vertex_cursor = 0;
			uint32_t index_data[MAX_IMMEDIATE_RENDERABLE_TRIS * 3];
			uint32_t index_cursor = 0;

			uint32_t viewport_width = 0;
			uint32_t viewport_height = 0;

			void init(uint32_t viewport_w, uint32_t viewport_h);
			void clear();
			void add_quad(float w, float h, float x, float y, float r = 1.f, float g = 1.f, float b = 1.f, float a = 1.f);
			// TODO_RENDER: Add more shapes later

			void set_vertex_data(ResourceHandle<Buffer> vertex_buffer, ResourceHandle<Buffer> index_buffer) override;
			void draw(const RenderParams& params) override;
		};

		struct MeshRenderable : Renderable
		{
            Mesh* mesh = nullptr;           // RTAO Only
            bool has_alpha_mask = false;  // RTAO Only

			void set_mesh_data(Mesh* mesh, ResourceHandle<Material> material_handle);
		};
	}
}