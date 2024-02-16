#include <SculptorPch.h>

#include "IndexBuffer.h"

#include "Buffer.h"
#include "StagingBuffer.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"

namespace Sculptor::Core
{
	IndexBuffer::IndexBuffer(std::weak_ptr<LogicalDevice> device)
		:	logicalDevice(std::move(device))
	{ }

	void IndexBuffer::Create(const void* bufferData, uint64_t bufferSize)
	{
		indexBufferProperties.bufferSize = bufferSize;
		indexBuffer.Create(indexBufferProperties);

		StagingBuffer stagingBuffer{ logicalDevice };
		stagingBuffer.CreateBuffer(bufferData, bufferSize);
		stagingBuffer.CopyBuffer(indexBuffer, bufferSize);
		stagingBuffer.DestroyBuffer();
	}

	void IndexBuffer::BindBuffer(VkCommandBuffer commandBuffer) const
	{
		vkCmdBindIndexBuffer(commandBuffer, indexBuffer.GetBuffer(), 0, VK_INDEX_TYPE_UINT16);
	}

	void IndexBuffer::Destroy() const
	{
		indexBuffer.Destroy();
	}
}
