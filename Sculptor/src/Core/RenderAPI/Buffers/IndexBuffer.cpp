#include <SculptorPch.h>

#include "IndexBuffer.h"

#include "Buffer.h"
#include "VertexBuffer.h"
#include "Core/Locators/LogicalDeviceLocator.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Core/Locators/CommandPoolLocator.h"
#include "Data/Constants.h"
#include "Utilities/BufferUtility.h"

namespace Sculptor::Core
{
	void IndexBuffer::Create(const BufferProperties& bufferProperties)
	{
		LOGICAL_DEVICE_LOCATOR

		Buffer stagingBuffer{};
		stagingBuffer.Create(bufferProperties);

		void* data;
		vkMapMemory(device, stagingBuffer.GetBufferMemory(), 0, bufferProperties.bufferSize, 0, &data);
		memcpy(data, INDICES.data(), static_cast<size_t>(bufferProperties.bufferSize));
		vkUnmapMemory(device, stagingBuffer.GetBufferMemory());

		const BufferProperties indexBufferProperties{
			bufferProperties.bufferSize,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		};

		Buffer::Create(indexBufferProperties);

		COMMAND_POOL_LOCATOR

		BufferUtility::CopyBuffer(cmdPool, device, stagingBuffer, *this, indexBufferProperties.bufferSize);

		stagingBuffer.Destroy();
	}

	void IndexBuffer::BindBuffer(VkCommandBuffer commandBuffer) const
	{
		vkCmdBindIndexBuffer(commandBuffer, buffer, 0, VK_INDEX_TYPE_UINT16);
	}
}
