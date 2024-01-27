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

		VertexBuffer(uint64_t bufferSize);

		~VertexBuffer() = default;

		void Create(const BufferProperties& bufferProperties);

		void Create(uint64_t bufferSize);

		void BindBufferMemory() const;

		void Bind(const VkCommandBuffer& cmdBuffer) const;

		void CleanUp() const;

		LOGICAL_DEVICE

		static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	private:
		VkBuffer buffer{};

		VkDeviceMemory bufferMemory{};

		Buffer vertexBuffer;

		void AllocateMemory(const VkMemoryRequirements& memoryRequirements);

		void MapMemory(const VkBufferCreateInfo& bufferInfo, const void* data) const;

		void UnMapMemory() const;
	};
}
