#include <SculptorPch.h>

#include "VulkanSemaphoreWrapper.h"

#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	VulkanSemaphoreWrapper::VulkanSemaphoreWrapper(const std::weak_ptr<LogicalDevice>& device)
		:	logicalDevice(device)
	{ }

	void VulkanSemaphoreWrapper::Create()
	{
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		const auto logicalDevicePtr = logicalDevice.lock();
		if (!logicalDevicePtr)
		{
			std::cerr << "Failed to create semaphore. Logical Device reference is null." << std::endl;
			return;
		}
		const auto& device = logicalDevicePtr->Get();

		const auto result = vkCreateSemaphore(device, &semaphoreInfo, nullptr, &semaphore);
		S_ASSERT(result != VK_SUCCESS, "Failed to create semaphore.");
	}

	void VulkanSemaphoreWrapper::Destroy() const
	{
		const auto logicalDevicePtr = logicalDevice.lock();
		if (!logicalDevicePtr)
		{
			std::cerr << "Failed to create fence. Logical Device reference is null." << std::endl;
			return;
		}
		const auto& device = logicalDevicePtr->Get();

		vkDestroySemaphore(device, semaphore, nullptr);
	}

	const VkSemaphore& VulkanSemaphoreWrapper::Get() const
	{
		return semaphore;
	}

	void VulkanSemaphoreWrapper::SetLogicalDevice(const std::weak_ptr<LogicalDevice>& device)
	{
		this->logicalDevice = device;
	}
}
