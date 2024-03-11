#include <SculptorPch.h>

#include "Buffer.h"

#include "VertexBuffer.h"
#include "Core/Core.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Core/Locators/LogicalDeviceLocator.h"
#include "Core/RenderAPI/Utility/CreateInfo.h"
#include "Utilities/BufferUtility.h"
#include "Utilities/GetShared.h"

namespace Sculptor::Core
{
	Buffer::Buffer()
		:	buffer{ VK_NULL_HANDLE },
			bufferMemory{ VK_NULL_HANDLE }
	{ }

	void Buffer::Create(const BufferProperties& properties)
	{
		LOGICAL_DEVICE_LOCATOR

		GetShared physicalDevicePtr(logicalDevicePtr->GetPhysicalDevice());
		const auto physicalDevice = physicalDevicePtr->GetPrimaryDevice();

		const auto bufferInfo = CreateInfo<VkBufferCreateInfo>({
			.size = properties.bufferSize,
			.usage = properties.usageFlags,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 0,
			.pQueueFamilyIndices = VK_NULL_HANDLE
		});

		VK_CHECK(vkCreateBuffer(device, &bufferInfo, nullptr, &buffer), "Failed to create buffer!")

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

		const auto allocInfo = CreateInfo<VkMemoryAllocateInfo>({
			.allocationSize = memRequirements.size,
			.memoryTypeIndex = BufferUtility::FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties.propertyFlags)
		});

		VK_CHECK(vkAllocateMemory(device, &allocInfo, VK_NULL_HANDLE, &bufferMemory), "Failed to allocate buffer memory.")

		BindBufferMemory();
	}

	void Buffer::Destroy() const
	{
		LOGICAL_DEVICE_LOCATOR

		vkDestroyBuffer(device, buffer, nullptr);
		vkFreeMemory(device, bufferMemory, nullptr);
	}

	void Buffer::BindBufferMemory(VkDeviceSize bufferSize /* = 0 */) const
	{
		LOGICAL_DEVICE_LOCATOR

		vkBindBufferMemory(device, buffer, bufferMemory, bufferSize);
	}

	VkBuffer& Buffer::GetBuffer()
	{
		return buffer;
	}

	const VkBuffer& Buffer::GetBuffer() const
	{
		return buffer;
	}

	VkDeviceMemory& Buffer::GetBufferMemory()
	{
		return bufferMemory;
	}

	const VkDeviceMemory& Buffer::GetBufferMemory() const
	{
		return bufferMemory;
	}
}
