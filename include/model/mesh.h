#pragma once

#include "tiny_obj_loader.h"
#include "debugging/assert.h"

class Mesh
{
public:
	static tinyobj::ObjReader meshReader;

	std::vector<float> vertices;
	// TODO: normals, uvs
	std::vector<uint32_t> indices;

	void LoadMesh(const char* path);
};
