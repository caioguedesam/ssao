#define TINYOBJLOADER_IMPLEMENTATION	// Only define this here.
#define STB_IMAGE_IMPLEMENTATION		// Only define this here.

#include "resource/resource_loader.h"

tinyobj::ObjReader ResourceLoader::meshReader;

void ResourceLoader::LoadModel(Model& targetModel, const char* path)
{
	targetModel.vertices.clear();
	targetModel.indices.clear();

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

				targetModel.vertices.push_back(vx);
				targetModel.vertices.push_back(vy);
				targetModel.vertices.push_back(vz);

				if (idx.normal_index >= 0)
				{
					tinyobj::real_t nx = attrib.vertices[3 * size_t(idx.normal_index) + 0];
					tinyobj::real_t ny = attrib.vertices[3 * size_t(idx.normal_index) + 1];
					tinyobj::real_t nz = attrib.vertices[3 * size_t(idx.normal_index) + 2];

					targetModel.vertices.push_back(nx);
					targetModel.vertices.push_back(ny);
					targetModel.vertices.push_back(nz);
				}
				else
				{
					ASSERT(0, "No normals data in obj model");
				}

				if (idx.texcoord_index >= 0) {
					tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
					tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];

					targetModel.vertices.push_back(tx);
					targetModel.vertices.push_back(ty);
				}
				else
				{
					ASSERT(0, "No UV data in obj model");
				}

				targetModel.indices.push_back(currentIndex++);
			}
			indexOffset += 3;
		}
	}
}

void ResourceLoader::LoadTexture(Texture& targetTexture, const char* path)
{
	int w, h, nC;
	unsigned char* imgData = stbi_load(path, &w, &h, &nC, 0);
	targetTexture.Init(w, h, nC, imgData, Texture::CreationFlags::NONE);
	stbi_image_free(imgData);
}