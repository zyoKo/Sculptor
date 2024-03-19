#pragma once

#include "Buffer.h"
#include "Structures/BufferProperties.h"

namespace Sculptor::Core
{
	class StagingBuffer
	{
	public:
		StagingBuffer() = default;

		StagingBuffer(std::weak_ptr<LogicalDevice> device) noexcept;

		void CreateBuffer(const void* bufferData, uint64_t bufferSize);

		void CopyBuffer(const Buffer& destinationBuffer, uint64_t bufferSize) const;

		void DestroyBuffer() const;

		LOGICAL_DEVICE

	private:
		Buffer stagingBuffer;

		inline static BufferProperties bufferCreationProperties{
			0,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT };
	};
}
