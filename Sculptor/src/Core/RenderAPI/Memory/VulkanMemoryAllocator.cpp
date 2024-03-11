#include <SculptorPch.h>

#include "VulkanMemoryAllocator.h"

#include "Core/Data/Constants.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	VulkanMemoryAllocator::VulkanMemoryAllocator(const std::weak_ptr<VulkanInstanceWrapper>& instance,
		const std::weak_ptr<LogicalDevice>& device)
		:	vulkanInstanceWrapper(instance),
			logicalDevice(device)
	{ }

	void VulkanMemoryAllocator::Create()
	{
		HANDLE_WEAK_VULKAN_INSTANCE_WRAPPER

		HANDLE_WEAK_LOGICAL_DEVICE_DEPRECATED

		VmaAllocatorCreateInfo vmaInfo{};
		vmaInfo.device = logicalDevice.lock()->Get();
		vmaInfo.instance = vulkanInstanceWrapper.lock()->GetInstance();
		vmaInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;	// if disabled will only use Vulkan 1.0 core with no extensions
		vmaInfo.vulkanApiVersion = VK_MAKE_VERSION(API::MAJOR, API::MINOR, API::PATCH);

		const VkResult result = vmaCreateAllocator(&vmaInfo, &memoryAllocator);
		S_ASSERT(result != VK_SUCCESS, "Failed to create vulkan memory allocator.");
	}

	const VmaAllocator& VulkanMemoryAllocator::Get() const
	{
		return memoryAllocator;
	}

	void VulkanMemoryAllocator::CleanUp() const
	{
		vmaDestroyAllocator(memoryAllocator);
	}
}
