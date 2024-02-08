#include <SculptorPch.h>

#include "VertexBuffer.h"

#include "Core/Locators/LogicalDeviceLocator.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Data/Constants.h"
#include "Structures/Vertex.h"
#include "Utilities/GetShared.h"
#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	VertexBuffer::VertexBuffer(const std::weak_ptr<LogicalDevice>& device)
		:	logicalDevice(device)
	{ }

	VertexBuffer::VertexBuffer(const BufferProperties& bufferProperties)
	{
		Create(bufferProperties);
	}

	void VertexBuffer::Create(const BufferProperties& bufferProperties)
	{
		LOGICAL_DEVICE_LOCATOR

		Buffer stagingBuffer{};
		stagingBuffer.Create(bufferProperties);

		void* data;
		vkMapMemory(device, stagingBuffer.GetBufferMemory(), 0, bufferProperties.bufferSize, 0, &data);
		memcpy(data, VERTICES.data(), static_cast<size_t>(bufferProperties.bufferSize));
		vkUnmapMemory(device, stagingBuffer.GetBufferMemory());
		
		const BufferProperties properties{
			bufferProperties.bufferSize,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		};
		
		vertexBuffer.Create(properties);
		
		Buffer::Copy(stagingBuffer, vertexBuffer, bufferProperties.bufferSize);
		
		stagingBuffer.Destroy();
	}

	void VertexBuffer::BindBufferMemory() const
	{
		HANDLE_WEAK_LOGICAL_DEVICE_DEPRECATED

		// If the memory offset is non-zero it needs to be divisible by memoryRequirements.alignment
		const auto& buffer = vertexBuffer.GetBuffer();
		const auto& bufferMemory = vertexBuffer.GetBufferMemory();
		vkBindBufferMemory(device, buffer, bufferMemory, 0);
	}

	void VertexBuffer::Bind(const VkCommandBuffer& cmdBuffer) const
	{
		//const VkBuffer vertexBuffers[] = { buffer };
		const VkBuffer vertexBuffers[] = { vertexBuffer.GetBuffer() };
		constexpr VkDeviceSize offsets[] = { 0 };

		vkCmdBindVertexBuffers(cmdBuffer, 0, 1, vertexBuffers, offsets);
	}

	void VertexBuffer::CleanUp() const
	{
		HANDLE_WEAK_LOGICAL_DEVICE_DEPRECATED

		vertexBuffer.Destroy();
	}

	void VertexBuffer::AllocateMemory(const VkMemoryRequirements& memoryRequirements)
	{
		HANDLE_WEAK_LOGICAL_DEVICE_DEPRECATED

		GetShared physicalDeviceObject(logicalDevicePtr->GetPhysicalDevice());
		const auto physicalDevice = physicalDeviceObject->GetPrimaryDevice();

		constexpr auto memoryTypeFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		const auto& memoryType = BaseBuffer::FindMemoryType(physicalDevice, memoryRequirements.memoryTypeBits, memoryTypeFlags);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.pNext = VK_NULL_HANDLE;
		allocInfo.allocationSize = memoryRequirements.size;
		allocInfo.memoryTypeIndex = memoryType;

		const VkResult result = vkAllocateMemory(device, &allocInfo, nullptr, &vertexBuffer.GetBufferMemory());
		S_ASSERT(result != VK_SUCCESS, "Failed to allocate memory for the vertex buffer.");
	}

	void VertexBuffer::MapMemory(const VkBufferCreateInfo& bufferInfo, const void* data) const
	{
		HANDLE_WEAK_LOGICAL_DEVICE_DEPRECATED

		void* newData = nullptr;

		vkMapMemory(device, vertexBuffer.GetBufferMemory(), 0, bufferInfo.size, 0, &newData);

		memcpy(newData, data, static_cast<size_t>(bufferInfo.size));
	}

	void VertexBuffer::UnMapMemory() const
	{
		HANDLE_WEAK_LOGICAL_DEVICE_DEPRECATED

		vkUnmapMemory(device, vertexBuffer.GetBufferMemory());
	}
}
