#pragma once

#include <glm/glm.hpp>

namespace Sculptor::Core
{
	struct Vertex
	{
		glm::vec2 pos;
		glm::vec3 color;

		static VkVertexInputBindingDescription GetBindingDescription(const uint32_t bindingPoint, const uint32_t stride)
		{
			// input rate has another parameter VK_VERTEX_INPUT_RATE_INSTANCE for instance rendering
			VkVertexInputBindingDescription bindingDesc{};
			bindingDesc.binding = bindingPoint;
			bindingDesc.stride = stride;
			bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDesc;
		}

		static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescription()
		{
			std::array<VkVertexInputAttributeDescription, 2> attributeDesc{};
			attributeDesc[0].binding = 0;
			attributeDesc[0].location = 0;
			attributeDesc[0].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDesc[0].offset = offsetof(Vertex, pos);

			attributeDesc[1].binding = 0;
			attributeDesc[1].location = 1;
			attributeDesc[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDesc[1].offset = offsetof(Vertex, color);

			return attributeDesc;
		}
	};
}