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
		VertexBuffer() = default;

		VertexBuffer(const std::weak_ptr<LogicalDevice>& device);

		~VertexBuffer() = default;

		void Create(const void* bufferData, uint64_t bufferSize);

		void BindBufferMemory() const;

		void Bind(const VkCommandBuffer& cmdBuffer) const;

		void CleanUp() const;

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
