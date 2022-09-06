#include "stdafx.h"
#include "resource/model_resource_manager.h"
#include "resource/shader_resource_manager.h"
#include "resource/texture_resource_manager.h"
#include "resource/material_resource_manager.h"
#include "globals.h"
#define TINYOBJLOADER_IMPLEMENTATION	// Only define this here.
#include "tiny_obj_loader.h"

namespace Ty
{
	namespace Graphics
	{
		ModelResourceManager model_resource_manager;

		void ModelResourceManager::init()
		{
			ResourceHandle<Shader> vs = shader_resource_manager.get_from_file(SHADERS_PATH"default_vs.vert");
			ResourceHandle<Shader> ps = shader_resource_manager.get_from_file(SHADERS_PATH"default_ps.frag");
			gbuffer_shader_pipeline = shader_resource_manager.create_linked_shader_pipeline(vs, ps);
			
			// TODO: Load all models from models folder
		}

		void ModelResourceManager::destroy() {}

		tinyobj::ObjReader mesh_reader;

		std::vector<ResourceHandle<Texture>> get_textures_from_names(const char* names[], uint32_t names_len)
		{
			std::vector<ResourceHandle<Texture>> handles;
			const char* texture_filename = names[0];
			for (uint32_t i = 0; i < names_len; i++)
			{
				char texture_filepath[MAX_PATH];
				ResourceHandle<Texture> texture_handle;
				const char* filename_with_ext = FileSystem::FilePath(texture_filename).get_file_name_with_ext();
				if (filename_with_ext)
				{
					sprintf(texture_filepath, "%s%s", TEXTURES_PATH, filename_with_ext);
					texture_handle = texture_resource_manager.load_from_file(texture_filepath);
				}
				handles.push_back(texture_handle);
				texture_filename = names[i + 1];
			}
			return handles;
		}

		ResourceHandle<Model> ModelResourceManager::load_from_file(const char* file_path)
		{
			ResourceHandle<Model> handle;
			FileSystem::FilePath path = FileSystem::FilePath(file_path);
			if (handle_list.count(path))
			{
				// Model already exists, don't load it again.
				return handle_list[path];
			}
			Model* model = new Model();
			handle = add(model);

			// Parse .obj file
			tinyobj::ObjReaderConfig read_config;
			read_config.triangulate = true;

			mesh_reader.ParseFromFile(file_path, read_config);
			ASSERT_FORMAT(mesh_reader.Error().empty(), "[ERROR:MODEL] Error reading model from path %s.", file_path);

			auto& shapes = mesh_reader.GetShapes();
			auto& attrib = mesh_reader.GetAttrib();
			auto& mats = mesh_reader.GetMaterials();

			// Create all materials
			for (uint32_t mat_index = 0; mat_index < mats.size(); mat_index++)
			{
				auto& mat = mats[mat_index];
				const char* gbuffer_texture_filenames[5] =
				{
					mat.ambient_texname.c_str(),
					mat.diffuse_texname.c_str(),
					mat.specular_texname.c_str(),
					mat.bump_texname.c_str(),
					mat.alpha_texname.c_str(),
				};
				std::vector<ResourceHandle<Texture>> gbuffer_textures = get_textures_from_names(gbuffer_texture_filenames, 5);

				ResourceHandle<Material> material_handle = material_resource_manager.create_material(mat.name.c_str(), gbuffer_shader_pipeline, gbuffer_textures);
				ASSERT(material_handle.is_valid(), "[ERROR:MODEL] Failed to initialize material.");
			}

			// TODO_RENDER: Load and allocate all vertex/index data for each mesh, pair them up with created materials, register them to RM

			for (size_t s = 0; s < shapes.size(); s++)
			{
				uint32_t index = 0;
				auto& tinyobj_mesh = shapes[s].mesh;
				Mesh* mesh = new Mesh();
				mesh->vertex_count = tinyobj_mesh.num_face_vertices.size() * 3;
				mesh->index_count = tinyobj_mesh.num_face_vertices.size() * 3;
				mesh->vertex_data = (ModelVertex*)malloc(mesh->vertex_count * sizeof(ModelVertex));
				mesh->index_data = (uint32_t*)malloc(mesh->index_count * sizeof(uint32_t));

				//size_t index_offset = 0;
				// For every face (faces are always assumed to be tris)
				for (size_t f = 0; f < tinyobj_mesh.num_face_vertices.size(); f++)
				{
					// For every vertex in face
					for (size_t v = 0; v < 3; v++)
					{
						uint32_t current_vertex = f * 3 + v;
						ModelVertex* model_vertex = &mesh->vertex_data[current_vertex];
						tinyobj::index_t idx = tinyobj_mesh.indices[current_vertex];

						model_vertex->px = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
						model_vertex->py = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
						model_vertex->pz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

						model_vertex->nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
						model_vertex->ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
						model_vertex->nz = attrib.normals[3 * size_t(idx.normal_index) + 2];

						model_vertex->tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
						model_vertex->ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];

						mesh->index_data[current_vertex] = current_vertex;		// Currently in sequential order, no memory optimizations here yet.

						/*target_model.vertices.push_back(vx);
						target_model.vertices.push_back(vy);
						target_model.vertices.push_back(vz);*/

						/*if (idx.normal_index >= 0)
						{
							tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
							tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
							tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];

							target_model.vertices.push_back(nx);
							target_model.vertices.push_back(ny);
							target_model.vertices.push_back(nz);
						}
						else
						{
							ASSERT(0, "No normals data in obj model");
						}*/
						//tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
						//tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
						//tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
						//mesh->vertex_data[index_offset + 3] = nx;
						//mesh->vertex_data[index_offset + 4] = ny;
						//mesh->vertex_data[index_offset + 5] = nz;

						/*if (idx.texcoord_index >= 0) {
							tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
							tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];

							target_model.vertices.push_back(tx);
							target_model.vertices.push_back(ty);
						}
						else
						{
							ASSERT(0, "No UV data in obj model");
						}*/
						//tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
						//tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
						//mesh->vertex_data[index_offset + 6] = tx;
						//mesh->vertex_data[index_offset + 7] = ty;

						/*target_model.indices.push_back(index++);*/
						//mesh->index_data[index_offset + 0] = index_offset + 0;
						//mesh->index_data[index_offset + 1] = index_offset + 1;
						//mesh->index_data[index_offset + 2] = index_offset + 2;
					}
					//index_offset += 3;
				}

				get(handle)->add_new_part(mesh, material_resource_manager.get_material(mats[tinyobj_mesh.material_ids[0]].name.c_str()));
			}

			handle_list[path] = handle;
			return handle;
		}

		ResourceHandle<Model> ModelResourceManager::get_from_file(const char* file_path)
		{
			FileSystem::FilePath path(file_path);
			ASSERT(handle_list.count(path), "Trying to get shader from file that was not compiled yet.");
			return handle_list[path];
		}
	}
}