#pragma once
#include "Utilities/Macros.h"

namespace Sculptor::Core
{
	class LogicalDevice;
}

namespace Sculptor::Core
{
	class VulkanSemaphoreWrapper
	{
	public:
		VulkanSemaphoreWrapper() noexcept;

		VulkanSemaphoreWrapper(std::weak_ptr<LogicalDevice> device) noexcept;

		VulkanSemaphoreWrapper(VulkanSemaphoreWrapper&) = delete;

		VulkanSemaphoreWrapper(VulkanSemaphoreWrapper&&) = default;

		~VulkanSemaphoreWrapper() = default;

		void Create();

		void Destroy() const;

		const VkSemaphore& Get() const;

		LOGICAL_DEVICE

	private:
		VkSemaphore semaphore;
	};
}
