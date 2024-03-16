#include <SculptorPch.h>

#include "Mesh.h"

#include <tiny_obj_loader.h>

#include "Core/RenderAPI/Buffers/VertexBuffer.h"
#include "Core/RenderAPI/Buffers/IndexBuffer.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"

namespace Sculptor::Component
{
	Mesh::Mesh(std::weak_ptr<Core::LogicalDevice> device, const std::string& filePath /* = VIKING_ROOM */)
		:	logicalDevice(std::move(device)),
			vertexBuffer(std::make_shared<Core::VertexBuffer>()),
			indexBuffer(std::make_shared<Core::IndexBuffer>())
	{
		LoadMesh(filePath);

		vertexBuffer->SetLogicalDevice(logicalDevice);
		indexBuffer->SetLogicalDevice(logicalDevice);
	}

	void Mesh::CreateBuffers() const
	{
		GetShared<Core::LogicalDevice> logicalDevicePtr{ logicalDevice };
		S_ASSERT(logicalDevicePtr->Get() == VK_NULL_HANDLE, "Create Logical Device Before Creating Vertex and Index Buffers.")

		// Vertex Buffer
		const uint64_t bufferSize = sizeof(vertices[0]) * vertices.size();
		vertexBuffer->Create(vertices.data(), bufferSize);

		// Index Buffer
		const uint64_t indexBufferSize = sizeof(indices[0]) * indices.size();
		indexBuffer->Create(indices.data(), indexBufferSize);
	}

	const std::vector<Core::Vertex>& Mesh::GetVertices() const
	{
		return vertices;
	}

	const std::vector<U32>& Mesh::GetIndices() const
	{
		return indices;
	}

	std::weak_ptr<Core::VertexBuffer> Mesh::GetVertexBuffer() const
	{
		return vertexBuffer;
	}

	std::weak_ptr<Core::IndexBuffer> Mesh::GetIndexBuffer() const
	{
		return indexBuffer;
	}

	void Mesh::CleanUp() const
	{
		vertexBuffer->Destroy();
		indexBuffer->Destroy();
	}

	void Mesh::LoadMesh(const std::string& filePath)
	{
		tinyobj::attrib_t tinyAttribute;

		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string warning, error;

		S_ASSERT(tinyobj::LoadObj(&tinyAttribute, &shapes, &materials, &warning, &error, filePath.c_str()) == false, "Failed to load model {0}", filePath)

		std::unordered_map<Core::Vertex, U32> uniqueVertices{};

		for (const auto& shape : shapes)
		{
			for (const auto& index : shape.mesh.indices)
			{
				Core::Vertex vertex{
					.pos = {
						tinyAttribute.vertices[3 * index.vertex_index + 0],
						tinyAttribute.vertices[3 * index.vertex_index + 1],
						tinyAttribute.vertices[3 * index.vertex_index + 2]
					},

					.color = { 1.0f, 1.0f, 1.0f },

					.texCoord = {
						tinyAttribute.texcoords[2 * index.texcoord_index + 0],
						1.0f - tinyAttribute.texcoords[2 * index.texcoord_index + 1]
					}
				};

				if (!uniqueVertices.contains(vertex)) 
				{
                    uniqueVertices[vertex] = static_cast<U32>(vertices.size());
                    vertices.push_back(vertex);
                }
				
                indices.push_back(uniqueVertices[vertex]);
			}
		}
	}
}
