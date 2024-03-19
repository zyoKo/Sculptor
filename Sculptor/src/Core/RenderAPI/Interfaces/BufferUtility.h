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
		static uint32_t FindMemoryType(const VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

		static void CopyBuffer(const Buffer& source, const Buffer& destination, VkDeviceSize size);
	};
}
