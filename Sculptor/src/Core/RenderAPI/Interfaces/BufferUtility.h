#pragma once

#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	class BufferUtility
	{
	public:
		static uint32_t FindMemoryType(const VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
		{
			VkPhysicalDeviceMemoryProperties memProperties{};
			vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

			for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
			{
				if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
				{
					return i;
				}
			}

			S_ASSERT(true, "Failed to find suitable memory type.");
			return -1;	// should never reach here
		}
	};
}
