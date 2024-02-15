#include <SculptorPch.h>

#include "Buffer.h"

#include "VertexBuffer.h"
#include "Core/Core.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Core/Locators/LogicalDeviceLocator.h"
#include "Utilities/GetShared.h"

namespace Sculptor::Core
{
	void Buffer::Create(const BufferProperties& properties)
	{
		LOGICAL_DEVICE_LOCATOR

		GetShared physicalDevicePtr(logicalDevicePtr->GetPhysicalDevice());
		const auto physicalDevice = physicalDevicePtr->GetPrimaryPhysicalDevice();

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
			FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties.propertyFlags)
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

	void Buffer::BindBufferMemory() const
	{
		LOGICAL_DEVICE_LOCATOR

		vkBindBufferMemory(device, buffer, bufferMemory, 0);
	}
}
