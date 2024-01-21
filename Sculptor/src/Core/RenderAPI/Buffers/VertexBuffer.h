#pragma once

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

		void Create(uint64_t bufferSize);

		void BindBufferMemory() const;

		void Bind(const VkCommandBuffer& cmdBuffer) const;

		void CleanUp() const;

		LOGICAL_DEVICE

	private:
		VkBuffer buffer{};

		VkDeviceMemory bufferMemory{};

		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

		void AllocateMemory(const VkMemoryRequirements& memoryRequirements);

		void MapMemory(const VkBufferCreateInfo& bufferInfo, const void* data) const;

		void UnMapMemory() const;
	};
}
