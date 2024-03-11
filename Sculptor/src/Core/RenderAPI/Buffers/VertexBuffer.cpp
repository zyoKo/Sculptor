#include <SculptorPch.h>

#include "VertexBuffer.h"

#include "StagingBuffer.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Core/RenderAPI/Interfaces/BufferUtility.h"
#include "Utilities/GetShared.h"
#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	VertexBuffer::VertexBuffer(std::weak_ptr<LogicalDevice> device)
		:	logicalDevice(std::move(device))
	{ }

	void VertexBuffer::Create(const void* bufferData, uint64_t bufferSize)
	{
		std::cout << "Creating VertexBuffer!\n";

		vertexBufferProperties.bufferSize = bufferSize;
		vertexBuffer.Create(vertexBufferProperties);
		
		StagingBuffer stagingBuffer{ logicalDevice };
		stagingBuffer.CreateBuffer(bufferData, bufferSize);
		stagingBuffer.CopyBuffer(vertexBuffer, bufferSize);
		stagingBuffer.DestroyBuffer();
	}

	void VertexBuffer::BindBufferMemory(VkDeviceSize bufferSize) const
	{
		HANDLE_WEAK_LOGICAL_DEVICE_DEPRECATED

		// If the memory offset is non-zero it needs to be divisible by memoryRequirements.alignment
		vkBindBufferMemory(device, vertexBuffer.buffer, vertexBuffer.bufferMemory, bufferSize);
	}

	void VertexBuffer::BindBuffer(VkCommandBuffer cmdBuffer) const
	{
		const VkBuffer vertexBuffers[] = { vertexBuffer.buffer };
		constexpr VkDeviceSize offsets[] = { 0 };

		vkCmdBindVertexBuffers(cmdBuffer, 0, 1, vertexBuffers, offsets);
	}

	void VertexBuffer::Destroy() const
	{
		vertexBuffer.Destroy();
	}

	void VertexBuffer::AllocateMemory(const VkMemoryRequirements& memoryRequirements)
	{
		HANDLE_WEAK_LOGICAL_DEVICE_DEPRECATED

		GetShared physicalDeviceObject(logicalDevicePtr->GetPhysicalDevice());
		const auto physicalDevice = physicalDeviceObject->GetPrimaryDevice();

		constexpr auto memoryTypeFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		const auto& memoryType = BufferUtility::FindMemoryType(physicalDevice, memoryRequirements.memoryTypeBits, memoryTypeFlags);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.pNext = VK_NULL_HANDLE;
		allocInfo.allocationSize = memoryRequirements.size;
		allocInfo.memoryTypeIndex = memoryType;

		const VkResult result = vkAllocateMemory(device, &allocInfo, nullptr, &vertexBuffer.bufferMemory);
		S_ASSERT(result != VK_SUCCESS, "Failed to allocate memory for the vertex buffer.")
	}

	void VertexBuffer::MapMemory(const VkBufferCreateInfo& bufferInfo, const void* data) const
	{
		HANDLE_WEAK_LOGICAL_DEVICE_DEPRECATED

		void* newData = nullptr;

		VK_CHECK(vkMapMemory(device, vertexBuffer.bufferMemory, 0, bufferInfo.size, 0, &newData),
			"Failed to Map Memory in Vertex Buffer!")

#ifdef DEBUG
		void* result = 
#endif
		memcpy(newData, data, static_cast<size_t>(bufferInfo.size));
	}

	void VertexBuffer::UnMapMemory() const
	{
		HANDLE_WEAK_LOGICAL_DEVICE_DEPRECATED

		vkUnmapMemory(device, vertexBuffer.bufferMemory);
	}
}
