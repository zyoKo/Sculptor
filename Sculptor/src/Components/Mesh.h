#pragma once

#include "Core/RenderAPI/Buffers/Structures/Vertex.h"
#include "Data/Constants.h"

namespace Sculptor::Component
{
	class Mesh
	{
	public:
		Mesh(const std::string& filePath = std::string(VIKING_ROOM));

		~Mesh() = default;

		const std::vector<Core::Vertex>& GetVertices() const;

		const std::vector<U32>& GetIndices() const;

	private:
		std::vector<Core::Vertex> vertices;

		std::vector<U32> indices;
	};
}
