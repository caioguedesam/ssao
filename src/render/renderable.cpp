#include "stdafx.h"
#include "resource/buffer_resource_manager.h"
#include "resource/shader_resource_manager.h"
#include "render/renderable.h"
#include <glad/glad.h>
#include "debugging/gl.h"
#include "globals.h"

namespace Ty
{
	namespace Graphics
	{
		void Renderable::set_vertex_data(ResourceHandle<Buffer> vertex_buffer, ResourceHandle<Buffer> index_buffer)
		{
			if (vao_api_handle == HANDLE_INVALID)
			{
				GL(glGenVertexArrays(1, &vao_api_handle));
			}

			this->vertex_buffer = vertex_buffer;
			this->index_buffer = index_buffer;

			GL(glBindVertexArray(vao_api_handle));

			buffer_resource_manager.bind_buffer(vertex_buffer);
			buffer_resource_manager.bind_buffer(index_buffer);

			{
				// For vertex buffers: vertex position (x, y, z) -> vertex normals (x, y, z) -> vertex UVs (u, v)
				GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * buffer_resource_manager.get(vertex_buffer)->get_stride(), (void*)0));
				GL(glEnableVertexAttribArray(0));

				GL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * buffer_resource_manager.get(vertex_buffer)->get_stride(), (void*)(3 * sizeof(float))));
				GL(glEnableVertexAttribArray(1));

				GL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * buffer_resource_manager.get(vertex_buffer)->get_stride(), (void*)(6 * sizeof(float))));
				GL(glEnableVertexAttribArray(2));
			}

			GL(glBindVertexArray(0));
		}

		void Renderable::set_material(Material* mat)
		{
			material = mat;
		}

		void bind_standard_uniforms(ShaderPipeline shader_pipeline, const RenderParams& params)
		{
			// TODO_SHADER: Change "model" to "world" matrix. WVP is just a better name.
			shader_pipeline.set_uniform("uModel", params.model);
			shader_pipeline.set_uniform("uView", params.view);
			shader_pipeline.set_uniform("uProj", params.proj);
			shader_pipeline.set_uniform("uMV", params.view * params.model);
			shader_pipeline.set_uniform("uVP", params.proj * params.view);
			shader_pipeline.set_uniform("uMVP", params.proj * params.view * params.model);

			// TODO_SHADER: Get per uniform names from shader instead of this.
			shader_pipeline.set_uniform("tex0", 0);
			shader_pipeline.set_uniform("tex1", 1);
			shader_pipeline.set_uniform("tex2", 2);
			shader_pipeline.set_uniform("tex3", 3);
		}

		void Renderable::draw(const RenderParams& params)
		{
			material->bind();

			// TODO_SHADER: Make a better process for binding all this default stuff, uniform buffers, etc.
			// I like OGLDev's pipeline stuff.

			bind_standard_uniforms(material->shader_pipeline, params);

			GL(glBindVertexArray(vao_api_handle));

			GL(glDrawElements(GL_TRIANGLES, buffer_resource_manager.get(index_buffer)->get_count(), GL_UNSIGNED_INT, 0));
		}

		void ImmediateRenderable::init(uint32_t viewport_w, uint32_t viewport_h)
		{
			// Initialize vertex and index buffers
			uint32_t attr_size = sizeof(ImmediateVertexAttr) / sizeof(float);
			uint32_t vertex_count = MAX_IMMEDIATE_RENDERABLE_TRIS * 3;
			uint32_t index_count = MAX_IMMEDIATE_RENDERABLE_TRIS * 3;
			vertex_buffer = buffer_resource_manager.create_buffer(
				{
					BufferType::VERTEX_BUFFER,
					BufferFormat::R32_FLOAT,
					vertex_count * attr_size
				}, vertex_data
			);
			index_buffer = buffer_resource_manager.create_buffer(
				{
					BufferType::INDEX_BUFFER,
					BufferFormat::R32_UINT,
					index_count
				}, index_data
			);

			// Initialize default material and shader pipeline
			set_material(new Material());
			ResourceHandle<Shader> vs = shader_resource_manager.get_from_file(SHADERS_PATH"immediate_vs.vert");
			ResourceHandle<Shader> ps = shader_resource_manager.get_from_file(SHADERS_PATH"immediate_ps.frag");
			material->init("", shader_resource_manager.create_linked_shader_pipeline(vs, ps));

			this->viewport_width = viewport_w;
			this->viewport_height = viewport_h;

			printf("VertexData size: %lld\n", sizeof(vertex_data));
			printf("IndexData size: %lld\n", sizeof(index_data));
		}

		void ImmediateRenderable::clear()
		{
			memset(vertex_data, 0, sizeof(vertex_data));
			memset(index_data, 0, sizeof(index_data));
			vertex_cursor = 0;
			index_cursor = 0;
		}

		void ImmediateRenderable::add_quad(float w, float h, float x, float y, float r, float g, float b, float a)
		{
			uint32_t base = vertex_cursor;
			vertex_data[vertex_cursor++] = { x,		y,			0.f, r, g, b, a };
			vertex_data[vertex_cursor++] = { x,		(y + h),	0.f, r, g, b, a };
			vertex_data[vertex_cursor++] = { (x + w),	(y + h),	0.f, r, g, b, a };
			vertex_data[vertex_cursor++] = { (x + w),	y,			0.f, r, g, b, a };

			index_data[index_cursor++] = base + 0;
			index_data[index_cursor++] = base + 2;
			index_data[index_cursor++] = base + 1;
			index_data[index_cursor++] = base + 0;
			index_data[index_cursor++] = base + 3;
			index_data[index_cursor++] = base + 2;
		}

		void ImmediateRenderable::set_vertex_data(ResourceHandle<Buffer> vertex_buffer, ResourceHandle<Buffer> index_buffer)
		{
			if (vao_api_handle == HANDLE_INVALID)
			{
				GL(glGenVertexArrays(1, &vao_api_handle));
			}

			this->vertex_buffer = vertex_buffer;
			this->index_buffer = index_buffer;

			GL(glBindVertexArray(vao_api_handle));

			buffer_resource_manager.bind_buffer(vertex_buffer);
			buffer_resource_manager.bind_buffer(index_buffer);

			{
				// For vertex buffers (immediate): vertex position (x, y, z) -> vertex colors (r, g, b, a)
				GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ImmediateVertexAttr), (void*)0));
				GL(glEnableVertexAttribArray(0));

				GL(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ImmediateVertexAttr), (void*)(3 * sizeof(float))));
				GL(glEnableVertexAttribArray(1));
			}

			GL(glBindVertexArray(0));
		}

		void ImmediateRenderable::draw(const RenderParams& params)
		{
			// TODO_RENDER: These have to be created on renderable init, with proper size
			buffer_resource_manager.set_buffer_data(vertex_buffer, vertex_data);
			buffer_resource_manager.set_buffer_data(index_buffer, index_data);
			set_vertex_data(vertex_buffer, index_buffer);

			// Draw
			material->bind();
			// TODO_SHADER: Figure out how to deal with matrix pipeline on immediate renderable
			//bindStandardUniforms(material->shaderPipeline, params);
			material->shader_pipeline.set_uniform("viewportWidth", (int)viewport_width);
			material->shader_pipeline.set_uniform("viewportHeight", (int)viewport_height);
			GL(glBindVertexArray(vao_api_handle));
			GL(glDrawElements(GL_TRIANGLES, index_cursor, GL_UNSIGNED_INT, 0));
		}
	}
}