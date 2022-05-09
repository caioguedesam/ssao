#define TINYOBJLOADER_IMPLEMENTATION	// Only define this here.

#include "model/mesh.h"


tinyobj::ObjReader Mesh::meshReader;

void Mesh::LoadMesh(const char* path)
{
	tinyobj::ObjReaderConfig readConfig;
	readConfig.triangulate = true;

	meshReader.ParseFromFile(path, readConfig);
	ASSERT(meshReader.Error().empty(), "Error reading mesh from given path.");

	auto& shapes = meshReader.GetShapes();
	auto& attrib = meshReader.GetAttrib();

	uint32_t currentIndex = 0;
	for (size_t s = 0; s < shapes.size(); s++)
	{
		size_t indexOffset = 0;
		// For every face (faces are always assumed to be tris)
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			// For every vertex in face
			for (size_t v = 0; v < 3; v++)
			{
				tinyobj::index_t idx = shapes[s].mesh.indices[indexOffset + v];

				tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
				tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
				tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

				vertices.push_back(vx);
				vertices.push_back(vy);
				vertices.push_back(vz);

				if (idx.texcoord_index >= 0) {
					tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
					tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];

					vertices.push_back(tx);
					vertices.push_back(ty);
				}
				else
				{
					ASSERT(0, "No UV data in obj model");
				}

				indices.push_back(currentIndex++);
			}
			indexOffset += 3;
		}
	}
}