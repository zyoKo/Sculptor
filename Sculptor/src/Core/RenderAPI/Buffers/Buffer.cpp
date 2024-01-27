#include <SculptorPch.h>

#include "Buffer.h"

#include "VertexBuffer.h"
#include "Core/Locators/CommandPoolLocator.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Utilities/Logger/Assert.h"
#include "Core/Locators/LogicalDeviceLocator.h"
#include "Core/Locators/CommandPoolLocator.h"
#include "Core/RenderAPI/Buffers/CommandBuffer.h"

namespace Sculptor::Core
{
	void Buffer::Create(const BufferProperties& properties)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = properties.bufferSize;
		bufferInfo.usage = properties.usageFlags;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		LOGICAL_DEVICE_LOCATOR;

		VkResult result = vkCreateBuffer(device, &bufferInfo, nullptr, &buffer);
		S_ASSERT(result != VK_SUCCESS, "Failed to create buffer.");

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = VertexBuffer::FindMemoryType(memRequirements.memoryTypeBits, properties.propertyFlags);

		result = vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory);
		S_ASSERT(result != VK_SUCCESS, "Failed to allocate buffer memory.");

		vkBindBufferMemory(device, buffer, bufferMemory, 0);
	}

	void Buffer::Copy(const Buffer& source, const Buffer& destination, VkDeviceSize size)
	{
		COMMAND_POOL_LOCATOR

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = cmdPool;
		allocInfo.commandBufferCount = 1;

		LOGICAL_DEVICE_LOCATOR;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		const VkBufferCopy copyRegion{0, 0, size};
		vkCmdCopyBuffer(commandBuffer, source.Get(), destination.Get(), 1, &copyRegion);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		const auto& graphicsQueue = logicalDevicePtr->GetQueueFamilies().GetGraphicsQueue();

		vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(graphicsQueue);

		vkFreeCommandBuffers(device, cmdPool, 1, &commandBuffer);
	}

	const VkBuffer& Buffer::Get() const
	{
		return buffer;
	}

	const VkDeviceMemory& Buffer::GetBufferMemory() const
	{
		return bufferMemory;
	}

	void Buffer::Destroy() const
	{
		LOGICAL_DEVICE_LOCATOR;

		vkDestroyBuffer(device, buffer, nullptr);
		vkFreeMemory(device, bufferMemory, nullptr);
	}
}
