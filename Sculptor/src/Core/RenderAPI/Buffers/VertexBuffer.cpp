#include <SculptorPch.h>

#include "VertexBuffer.h"

#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Data/Constants.h"
#include "Structures/Vertex.h"
#include "Utilities/Logger/Assert.h"
#include "Core/RenderAPI/Buffers/CommandBuffer.h"

namespace Sculptor::Core
{
	VertexBuffer::VertexBuffer(const std::weak_ptr<LogicalDevice>& device)
		:	logicalDevice(device)
	{ }

	VertexBuffer::VertexBuffer(uint64_t bufferSize)
	{
		Create(bufferSize);
	}

	void VertexBuffer::Create(uint64_t bufferSize)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = bufferSize;
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		HANDLE_WEAK_LOGICAL_DEVICE

		const VkResult result = vkCreateBuffer(device, &bufferInfo, nullptr, &buffer);
		S_ASSERT(result != VK_SUCCESS, "Failed to create Vertex Buffer.");

		VkMemoryRequirements memRequirements{};
		vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

		AllocateMemory(memRequirements);

		BindBufferMemory();

		MapMemory(bufferInfo, VERTICES.data());
		UnMapMemory();
	}

	void VertexBuffer::BindBufferMemory() const
	{
		HANDLE_WEAK_LOGICAL_DEVICE

		// If the memory offset is non-zero it needs to be divisible by memoryRequirements.alignment
		vkBindBufferMemory(device, buffer, bufferMemory, 0);
	}

	void VertexBuffer::Bind(const VkCommandBuffer& cmdBuffer) const
	{
		const VkBuffer vertexBuffers[] = { buffer };
		constexpr VkDeviceSize offsets[] = { 0 };

		vkCmdBindVertexBuffers(cmdBuffer, 0, 1, vertexBuffers, offsets);
	}

	void VertexBuffer::CleanUp() const
	{
		HANDLE_WEAK_LOGICAL_DEVICE

		vkDestroyBuffer(device, buffer, nullptr);

		vkFreeMemory(device, bufferMemory, nullptr);
	}

	uint32_t VertexBuffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const
	{
		HANDLE_WEAK_LOGICAL_DEVICE

		const auto physicalDevicePtr = logicalDevicePtr->GetPhysicalDevice().lock();
		if (!physicalDevicePtr)
		{
			std::cerr << "Physical Device pointer is null.\n";
			std::cerr << "Failed to Find Memory for Vertex Buffer.\n";
			__debugbreak();
		}
		const auto& physicalDevice = physicalDevicePtr->GetPrimaryPhysicalDevice();

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

	void VertexBuffer::AllocateMemory(const VkMemoryRequirements& memoryRequirements)
	{
		HANDLE_WEAK_LOGICAL_DEVICE

		constexpr auto memoryTypeFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		const auto& memoryType = FindMemoryType(memoryRequirements.memoryTypeBits, memoryTypeFlags);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		//allocInfo.pNext = VK_NULL_HANDLE;
		allocInfo.allocationSize = memoryRequirements.size;
		allocInfo.memoryTypeIndex = memoryType;

		const VkResult result = vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory);
		S_ASSERT(result != VK_SUCCESS, "Failed to allocate memory for the vertex buffer.");
	}

	void VertexBuffer::MapMemory(const VkBufferCreateInfo& bufferInfo, const void* data) const
	{
		HANDLE_WEAK_LOGICAL_DEVICE

		void* newData = nullptr;

		vkMapMemory(device, bufferMemory, 0, bufferInfo.size, 0, &newData);

		memcpy(newData, data, static_cast<size_t>(bufferInfo.size));
	}

	void VertexBuffer::UnMapMemory() const
	{
		HANDLE_WEAK_LOGICAL_DEVICE

		vkUnmapMemory(device, bufferMemory);
	}
}
