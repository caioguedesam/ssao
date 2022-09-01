#include "stdafx.h"
#define TINYOBJLOADER_IMPLEMENTATION	// Only define this here.

#include "resource/resource_loader.h"

namespace Ty
{
	namespace AssetSystem
	{
		tinyobj::ObjReader ResourceLoader::mesh_reader;

		void ResourceLoader::load_model(Model& target_model, const char* path)
		{
			target_model.vertices.clear();
			target_model.indices.clear();

			tinyobj::ObjReaderConfig read_config;
			read_config.triangulate = true;

			mesh_reader.ParseFromFile(path, read_config);
			ASSERT(mesh_reader.Error().empty(), "Error reading mesh from given path.");

			auto& shapes = mesh_reader.GetShapes();
			auto& attrib = mesh_reader.GetAttrib();

			uint32_t index = 0;
			for (size_t s = 0; s < shapes.size(); s++)
			{
				size_t index_offset = 0;
				// For every face (faces are always assumed to be tris)
				for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
				{
					// For every vertex in face
					for (size_t v = 0; v < 3; v++)
					{
						tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

						tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
						tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
						tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

						target_model.vertices.push_back(vx);
						target_model.vertices.push_back(vy);
						target_model.vertices.push_back(vz);

						if (idx.normal_index >= 0)
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
						}

						if (idx.texcoord_index >= 0) {
							tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
							tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];

							target_model.vertices.push_back(tx);
							target_model.vertices.push_back(ty);
						}
						else
						{
							ASSERT(0, "No UV data in obj model");
						}

						target_model.indices.push_back(index++);
					}
					index_offset += 3;
				}
			}
		}
	}
}