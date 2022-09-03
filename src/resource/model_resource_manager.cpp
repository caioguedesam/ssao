#include "resource/model_resource_manager.h"
#include "resource/shader_resource_manager.h"
#include "resource/texture_resource_manager.h"
#include "resource/material_resource_manager.h"
#include "tiny_obj_loader.h"
#include "globals.h"
#define TINYOBJLOADER_IMPLEMENTATION	// Only define this here.

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
				
				const char* ambient_texture_filename = FileSystem::FilePath(mat.ambient_texname.c_str()).get_file_name_with_ext();
				const char* diffuse_texture_filename = FileSystem::FilePath(mat.diffuse_texname.c_str()).get_file_name_with_ext();
				const char* specular_texture_filename = FileSystem::FilePath(mat.specular_texname.c_str()).get_file_name_with_ext();
				const char* bump_texture_filename = FileSystem::FilePath(mat.bump_texname.c_str()).get_file_name_with_ext();
				const char* alpha_texture_filename = FileSystem::FilePath(mat.alpha_texname.c_str()).get_file_name_with_ext();
				
				// TODO_RENDER: Load all of the above textures if they exist for material, make vector to pass to create_material()

				/*texture_resource_manager.load_from_file(ambient_texture_filename);
				texture_resource_manager.load_from_file(diffuse_texture_filename);
				texture_resource_manager.load_from_file(specular_texture_filename);
				texture_resource_manager.load_from_file(bump_texture_filename);
				texture_resource_manager.load_from_file(alpha_texture_filename);*/

				/*material_resource_manager.create_material(mat.name.c_str(), gbuffer_shader_pipeline, )*/
			}

			// TODO_RENDER: Load and allocate all vertex/index data for each mesh, pair them up with created materials, register them to RM

			//uint32_t index = 0;
			//for (size_t s = 0; s < shapes.size(); s++)
			//{
			//	size_t index_offset = 0;
			//	// For every face (faces are always assumed to be tris)
			//	for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			//	{
			//		// For every vertex in face
			//		for (size_t v = 0; v < 3; v++)
			//		{
			//			tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

			//			tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
			//			tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
			//			tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

			//			target_model.vertices.push_back(vx);
			//			target_model.vertices.push_back(vy);
			//			target_model.vertices.push_back(vz);

			//			if (idx.normal_index >= 0)
			//			{
			//				tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
			//				tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
			//				tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];

			//				target_model.vertices.push_back(nx);
			//				target_model.vertices.push_back(ny);
			//				target_model.vertices.push_back(nz);
			//			}
			//			else
			//			{
			//				ASSERT(0, "No normals data in obj model");
			//			}

			//			if (idx.texcoord_index >= 0) {
			//				tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
			//				tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];

			//				target_model.vertices.push_back(tx);
			//				target_model.vertices.push_back(ty);
			//			}
			//			else
			//			{
			//				ASSERT(0, "No UV data in obj model");
			//			}

			//			target_model.indices.push_back(index++);
			//		}
			//		index_offset += 3;
			//	}
			//}
		/*}*/

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