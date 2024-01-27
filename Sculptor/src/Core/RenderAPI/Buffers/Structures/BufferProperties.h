#pragma once

namespace Sculptor::Core
{
	struct BufferProperties
	{
		VkDeviceSize bufferSize;

		VkBufferUsageFlags usageFlags;

		VkMemoryPropertyFlags propertyFlags;
	};
}