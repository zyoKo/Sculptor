#include <SculptorPch.h>

#include "Buffer.h"

#include "VertexBuffer.h"
#include "Core/Core.h"
#include "Core/Locators/CommandPoolLocator.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Utilities/Logger/Assert.h"
#include "Core/Locators/LogicalDeviceLocator.h"
#include "Core/Locators/CommandPoolLocator.h"

namespace Sculptor::Core
{
	void Buffer::Create(const BufferProperties& properties)
	{
		LOGICAL_DEVICE_LOCATOR;

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = properties.bufferSize;
		bufferInfo.usage = properties.usageFlags;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VK_CHECK(vkCreateBuffer(device, &bufferInfo, nullptr, &buffer), "Failed to create buffer! {0}", 1)

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = VertexBuffer::FindMemoryType(memRequirements.memoryTypeBits, properties.propertyFlags);

		VK_CHECK(vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory), "Failed to allocate buffer memory.");

		BindBufferMemory();
	}

	void Buffer::Copy(const Buffer& source, const Buffer& destination, VkDeviceSize size)
	{
		COMMAND_POOL_LOCATOR;

		LOGICAL_DEVICE_LOCATOR;

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = cmdPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		const VkBufferCopy copyRegion{0, 0, size};
		vkCmdCopyBuffer(commandBuffer, source.GetBuffer(), destination.GetBuffer(), 1, &copyRegion);

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

	void Buffer::Destroy() const
	{
		LOGICAL_DEVICE_LOCATOR;

		vkDestroyBuffer(device, buffer, nullptr);
		vkFreeMemory(device, bufferMemory, nullptr);
	}

	void Buffer::BindBufferMemory() const
	{
		LOGICAL_DEVICE_LOCATOR;

		vkBindBufferMemory(device, buffer, bufferMemory, 0);
	}
}
