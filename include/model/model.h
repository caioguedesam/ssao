#pragma once

#include "stdafx.h"

namespace Ty
{
	namespace AssetSystem
	{
		class Model
		{
		public:
			std::vector<float> vertices;
			// TODO: normals, uvs
			std::vector<uint32_t> indices;
		};
	}
}
