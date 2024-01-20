#include <SculptorPch.h>

#include "VulkanFenceWrapper.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	VulkanFenceWrapper::VulkanFenceWrapper(const std::weak_ptr<LogicalDevice>& device)
		:	logicalDevice(device)
	{ }

	void VulkanFenceWrapper::Create()
	{
		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;	// so it doesn't keep on waiting on the first call (indefinitely)

		const auto logicalDevicePtr = logicalDevice.lock();
		if (!logicalDevicePtr)
		{
			std::cerr << "Failed to create fence. Logical Device reference is null.\n";
			return;
		}
		const auto& device = logicalDevicePtr->Get();

		const auto result = vkCreateFence(device, &fenceInfo, nullptr, &fence);
		S_ASSERT(result != VK_SUCCESS, "Failed to create fence");
	}

	void VulkanFenceWrapper::Reset() const
	{
		const auto logicalDevicePtr = logicalDevice.lock();
		if (!logicalDevicePtr)
		{
			std::cerr << "Failed to reset fence. Logical Device reference is null.\n";
			return;
		}
		const auto& device = logicalDevicePtr->Get();

		vkResetFences(device, 1, &fence);
	}

	void VulkanFenceWrapper::Wait() const
	{
		const auto logicalDevicePtr = logicalDevice.lock();
		if (!logicalDevicePtr)
		{
			std::cerr << "Fence wait failed. Logical Device reference is null.\n";
			return;
		}
		const auto& device = logicalDevicePtr->Get();

		vkWaitForFences(device, 1, &fence, VK_TRUE, UINT32_MAX);
	}

	void VulkanFenceWrapper::Destroy() const
	{
		const auto logicalDevicePtr = logicalDevice.lock();
		if (!logicalDevicePtr)
		{
			std::cerr << "Failed to destroy fence. Logical Device reference is null.\n";
			return;
		}
		const auto& device = logicalDevicePtr->Get();

		vkDestroyFence(device, fence, nullptr);
	}

	const VkFence& VulkanFenceWrapper::Get() const
	{
		return fence;
	}

	void VulkanFenceWrapper::SetLogicalDevice(const std::weak_ptr<LogicalDevice>& device)
	{
		this->logicalDevice = device;
	}
}
