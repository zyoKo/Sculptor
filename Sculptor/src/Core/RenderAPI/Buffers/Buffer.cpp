#include <SculptorPch.h>

#include "Buffer.h"

#include "VertexBuffer.h"
#include "Core/Core.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Core/Locators/LogicalDeviceLocator.h"
#include "Utilities/BufferUtility.h"
#include "Utilities/GetShared.h"

namespace Sculptor::Core
{
	Buffer::Buffer()
		:	buffer{ nullptr },
			bufferMemory{ nullptr }
	{ }

	void Buffer::Create(const BufferProperties& properties)
	{
		LOGICAL_DEVICE_LOCATOR

		GetShared physicalDevicePtr(logicalDevicePtr->GetPhysicalDevice());
		const auto physicalDevice = physicalDevicePtr->GetPrimaryDevice();

		const VkBufferCreateInfo bufferInfo{
			VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			nullptr,
			0,
			properties.bufferSize,
			properties.usageFlags,
			VK_SHARING_MODE_EXCLUSIVE,
			0,
			nullptr
		};

		VK_CHECK(vkCreateBuffer(device, &bufferInfo, nullptr, &buffer), "Failed to create buffer!")

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

		const VkMemoryAllocateInfo allocInfo{
			VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			nullptr,
			memRequirements.size,
			BufferUtility::FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties.propertyFlags)
		};
		
		VK_CHECK(vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory), "Failed to allocate buffer memory.")

		BindBufferMemory();
	}

	void Buffer::Destroy() const
	{
		LOGICAL_DEVICE_LOCATOR

		vkDestroyBuffer(device, buffer, nullptr);
		vkFreeMemory(device, bufferMemory, nullptr);
	}

	void Buffer::BindBuffer(VkCommandBuffer commandBuffer) const
	{
		// Some buffer need not bind like VertexBuffer so,
		// they can override this function if needed
	}

	void Buffer::BindBufferMemory(VkDeviceSize bufferSize /* = 0 */) const
	{
		LOGICAL_DEVICE_LOCATOR

		vkBindBufferMemory(device, buffer, bufferMemory, bufferSize);
	}

	VkBuffer Buffer::GetBuffer() const
	{
		return buffer;
	}

	VkDeviceMemory Buffer::GetBufferMemory() const
	{
		return bufferMemory;
	}
}
