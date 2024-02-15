#include <SculptorPch.h>

#include "BufferUtility.h"

#include "Core/RenderAPI/Buffers/CommandBuffer.h"
#include "Utilities/Macros.h"
#include "Utilities/Logger/Assert.h"
#include "Core/Locators/CommandPoolLocator.h"
#include "Core/Locators/LogicalDeviceLocator.h"
#include "Core/RenderAPI/Buffers/Buffer.h"

namespace Sculptor::Core
{
	uint32_t BufferUtility::FindMemoryType(const VkPhysicalDevice& physicalDevice, uint32_t typeFilter,
		VkMemoryPropertyFlags properties)
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

	void BufferUtility::CopyBuffer(const Buffer& source, const Buffer& destination, VkDeviceSize size)
	{
		COMMAND_POOL_LOCATOR

		LOGICAL_DEVICE_LOCATOR

		const VkCommandBuffer commandBuffer = CommandBuffer::BeginSingleTimeCommand(cmdPool, device);

		const VkBufferCopy copyRegion{ 0, 0, size };
		vkCmdCopyBuffer(commandBuffer, source.GetBuffer(), destination.GetBuffer(), 1, &copyRegion);

		CommandBuffer::EndSingleTimeCommand(commandBuffer);
	}
}
