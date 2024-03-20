#include <SculptorPch.h>

#include "VulkanSemaphoreWrapper.h"

#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Core/RenderAPI/Utility/CreateInfo.h"

namespace Sculptor::Core
{
	VulkanSemaphoreWrapper::VulkanSemaphoreWrapper() noexcept
		:	semaphore(VK_NULL_HANDLE)
	{ }

	VulkanSemaphoreWrapper::VulkanSemaphoreWrapper(std::weak_ptr<LogicalDevice> device) noexcept
		:	logicalDevice(std::move(device)),
			semaphore(VK_NULL_HANDLE)
	{ }

	void VulkanSemaphoreWrapper::Create()
	{
		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto device = logicalDevicePtr->Get();

		const auto semaphoreCreateInfo = CreateInfo<VkSemaphoreCreateInfo>();

		VK_CHECK(vkCreateSemaphore(device, &semaphoreCreateInfo, VK_NULL_HANDLE, &semaphore), "Failed to create semaphore.")
	}

	void VulkanSemaphoreWrapper::Destroy() const
	{
		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto device = logicalDevicePtr->Get();

		vkDestroySemaphore(device, semaphore, VK_NULL_HANDLE);
	}

	const VkSemaphore& VulkanSemaphoreWrapper::Get() const
	{
		return semaphore;
	}
}
