#pragma once

#include "Core/RenderAPI/Buffers/Structures/Vertex.h"
#include "Components/Data/Constants.h"
#include "Utilities/Macros.h"

namespace Sculptor::Core
{
	class IndexBuffer;
	class VertexBuffer;
	class LogicalDevice;
}

namespace Sculptor::Component
{
	class Mesh
	{
	public:
		Mesh(std::weak_ptr<Core::LogicalDevice> device, const std::string& filePath = std::string(DEFAULT_MESH));

		~Mesh() = default;

		void CreateBuffers() const;

		const std::vector<Core::Vertex>& GetVertices() const;

		const std::vector<U32>& GetIndices() const;

		std::weak_ptr<Core::VertexBuffer> GetVertexBuffer() const;

		std::weak_ptr<Core::IndexBuffer> GetIndexBuffer() const;

		void CleanUp() const;

		LOGICAL_DEVICE

	private:
		std::vector<Core::Vertex> vertices;

		std::vector<U32> indices;

		std::shared_ptr<Core::VertexBuffer> vertexBuffer;

		std::shared_ptr<Core::IndexBuffer> indexBuffer;

		void LoadMesh(const std::string& filePath);
	};
}
