#pragma once

namespace Sculptor::Core
{
	class LogicalDevice;
}

namespace Sculptor::Core
{
	class VulkanFenceWrapper
	{
	public:
		VulkanFenceWrapper() = default;

		VulkanFenceWrapper(const std::weak_ptr<LogicalDevice>& device);

		~VulkanFenceWrapper() = default;

		void Create();

		void Reset() const;

		void Wait() const;

		void Destroy() const;

		const VkFence& Get() const;

		void SetLogicalDevice(const std::weak_ptr<LogicalDevice>& device);

	private:
		VkFence fence{};

		std::weak_ptr<LogicalDevice> logicalDevice;
	};
}
