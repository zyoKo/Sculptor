#pragma once

namespace Sculptor::Core
{
	class Buffer;
}

namespace Sculptor::Core
{
	class BufferUtility
	{
	public:
		static void CopyBuffer(VkCommandPool commandPool, VkDevice logicalDevice, const Buffer& source, const Buffer& destination, VkDeviceSize size);

		static uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
	};
}
