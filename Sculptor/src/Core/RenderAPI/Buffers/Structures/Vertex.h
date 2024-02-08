#pragma once

#include <glm/glm.hpp>

namespace Sculptor::Core
{
	struct Vertex
	{
		glm::vec2 pos;
		glm::vec3 color;
		glm::vec2 texCoord;

		static VkVertexInputBindingDescription GetBindingDescription(const uint32_t bindingPoint, const uint32_t stride)
		{
			// input rate has another parameter VK_VERTEX_INPUT_RATE_INSTANCE for instance rendering
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding		= bindingPoint;
			bindingDescription.stride		= stride;
			bindingDescription.inputRate	= VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescription()
		{
			std::array<VkVertexInputAttributeDescription, 3> attributeDescription{};
			attributeDescription[0].binding		= 0;
			attributeDescription[0].location	= 0;
			attributeDescription[0].format		= VK_FORMAT_R32G32_SFLOAT;
			attributeDescription[0].offset		= offsetof(Vertex, pos);
		
			attributeDescription[1].binding		= 0;
			attributeDescription[1].location	= 1;
			attributeDescription[1].format		= VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescription[1].offset		= offsetof(Vertex, color);

			attributeDescription[2].binding		= 0;
			attributeDescription[2].location	= 2;
			attributeDescription[2].format		= VK_FORMAT_R32G32_SFLOAT;
			attributeDescription[2].offset		= offsetof(Vertex, texCoord);
		
			return attributeDescription;
		}
	};
}