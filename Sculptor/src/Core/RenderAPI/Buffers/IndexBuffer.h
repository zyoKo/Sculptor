#pragma once

#include "Buffer.h"

namespace Sculptor::Core
{
	class IndexBuffer
	{
	public:
		IndexBuffer() = default;

		IndexBuffer(std::weak_ptr<LogicalDevice> device);

		void Create(const void* bufferData, uint64_t bufferSize);

		void BindBuffer(const VkCommandBuffer& commandBuffer) const;

		void CleanUp() const;

		LOGICAL_DEVICE

	private:
		Buffer indexBuffer{};

		inline static BufferProperties indexBufferProperties{
			0,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		};
	};
}
