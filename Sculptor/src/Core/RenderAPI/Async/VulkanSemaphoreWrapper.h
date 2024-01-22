#pragma once

namespace Sculptor::Core
{
	class LogicalDevice;
}

namespace Sculptor::Core
{
	class VulkanSemaphoreWrapper
	{
	public:
		VulkanSemaphoreWrapper() noexcept = default;

		VulkanSemaphoreWrapper(const std::weak_ptr<LogicalDevice>& device);

		~VulkanSemaphoreWrapper() = default;

		void Create();

		void Destroy() const;

		const VkSemaphore& Get() const;

		void SetLogicalDevice(const std::weak_ptr<LogicalDevice>& device);

	private:
		VkSemaphore semaphore{};

		std::weak_ptr<LogicalDevice> logicalDevice;
	};
}
