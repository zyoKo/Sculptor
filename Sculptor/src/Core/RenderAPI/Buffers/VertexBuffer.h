#pragma once

#include "Buffer.h"
#include "Structures/BufferProperties.h"
#include "Utilities/Macros.h"

namespace Sculptor::Core
{
	class LogicalDevice;
	class CommandBuffer;
}

namespace Sculptor::Core
{
	class VertexBuffer
	{
	public:
		~VertexBuffer() = default;

		VertexBuffer(std::weak_ptr<LogicalDevice> device);

		void Create(const void* bufferData, uint64_t bufferSize);

		void BindBufferMemory(VkDeviceSize bufferSize) const;

		void BindBuffer(VkCommandBuffer cmdBuffer) const;

		void Destroy() const;

		LOGICAL_DEVICE

	private:
		Buffer vertexBuffer;

		inline static BufferProperties vertexBufferProperties{
			0,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		};

		void AllocateMemory(const VkMemoryRequirements& memoryRequirements);

		void MapMemory(const VkBufferCreateInfo& bufferInfo, const void* data) const;

		void UnMapMemory() const;
	};
}
