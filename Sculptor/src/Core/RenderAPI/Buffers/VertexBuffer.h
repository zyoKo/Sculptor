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
	class VertexBuffer : public Buffer
	{
	public:
		VertexBuffer(const std::weak_ptr<LogicalDevice>& device);

		void Create(const BufferProperties& bufferProperties) override;

		void BindBufferMemory(VkDeviceSize bufferSize = 0) const override;

		void BindBuffer(VkCommandBuffer cmdBuffer) const override;

		LOGICAL_DEVICE

	private:
		void AllocateMemory(const VkMemoryRequirements& memoryRequirements);

		void MapMemory(const VkBufferCreateInfo& bufferInfo, const void* data) const;

		void UnMapMemory() const;
	};
}
