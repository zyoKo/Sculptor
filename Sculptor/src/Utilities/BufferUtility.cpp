#include <SculptorPch.h>

#include "BufferUtility.h"

#include "Utilities/Logger/Assert.h"
#include "Core/RenderAPI/Buffers/Buffer.h"
#include "Core/RenderAPI/Buffers/CommandBuffer.h"

namespace Sculptor::Core
{
	void BufferUtility::CopyBuffer(VkCommandPool commandPool, VkDevice logicalDevice, const Buffer& source, const Buffer& destination, VkDeviceSize size)
	{
		const VkCommandBuffer commandBuffer = CommandBuffer::BeginSingleTimeCommand(commandPool, logicalDevice);

		const VkBufferCopy copyRegion{ 0, 0, size };
		vkCmdCopyBuffer(commandBuffer, source.GetBuffer(), destination.GetBuffer(), 1, &copyRegion);

		CommandBuffer::EndSingleTimeCommand(commandBuffer);
	}

	uint32_t BufferUtility::FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
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

		S_ASSERT(true, "Failed to find suitable memory type.")
		return -1;	// should never reach here
	}
}
