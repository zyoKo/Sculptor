#include <SculptorPch.h>

#include "StagingBuffer.h"

#include "Core/Locators/LogicalDeviceLocator.h"

namespace Sculptor::Core
{
	StagingBuffer::StagingBuffer(std::weak_ptr<LogicalDevice> device) noexcept
		:	logicalDevice(std::move(device))
	{ }

	void StagingBuffer::CreateBuffer(const void* bufferData, uint64_t bufferSize)
	{
		GetShared<LogicalDevice> logicalDevicePtr { logicalDevice };
		const auto& device = logicalDevicePtr->Get();

		bufferCreationProperties.bufferSize = bufferSize;
		stagingBuffer.Create(bufferCreationProperties);

		void* data;
		vkMapMemory(device, stagingBuffer.GetBufferMemory(), 0, bufferSize, 0, &data);
		memcpy(data, bufferData, static_cast<size_t>(bufferSize));
		vkUnmapMemory(device, stagingBuffer.GetBufferMemory());
	}

	void StagingBuffer::DestroyBuffer() const
	{
		bufferCreationProperties.bufferSize = 0;

		stagingBuffer.Destroy();
	}

	void StagingBuffer::CopyBuffer(const Buffer& destinationBuffer, uint64_t bufferSize) const
	{
		BufferUtility::CopyBuffer(stagingBuffer, destinationBuffer, bufferSize);
	}
}
