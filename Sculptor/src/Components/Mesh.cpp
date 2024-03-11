#include <SculptorPch.h>

#include "Mesh.h"

#include <tiny_obj_loader.h>

namespace Sculptor::Component
{
	Mesh::Mesh(const std::string& filePath /* = VIKING_ROOM */)
	{
		tinyobj::attrib_t tinyAttribute;

		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string warning, error;

		S_ASSERT(tinyobj::LoadObj(&tinyAttribute, &shapes, &materials, &warning, &error, filePath.c_str()) == false, "Failed to load model {0}", filePath)

		//std::unordered_map<Core::Vertex, U32> uniqueVertices{};

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

	const std::vector<Core::Vertex>& Mesh::GetVertices() const
	{
		return vertices;
	}

	const std::vector<U32>& Mesh::GetIndices() const
	{
		return indices;
	}
}
