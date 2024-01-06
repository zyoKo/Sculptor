#pragma once

#include "Core/RenderAPI/ValidationLayer/ValidationLayer.h"
#include "QueueFamily/QueueFamilies.h"

namespace Sculptor::Core
{
	class SwapChains;
}

namespace Sculptor::Core
{
	class LogicalDevice
	{
	public:
		LogicalDevice();

		~LogicalDevice() = default;

		void CreateLogicalDevice(
			const std::shared_ptr<VulkanInstanceWrapper>& vulkanInstanceWrapper,
			const std::shared_ptr<ValidationLayer>& validationLayer, 
			const std::shared_ptr<Windows::VulkanWindowSurface>& vulkanWindowSurface);

		const VkDevice& GetLogicalDevice() const;

		const QueueFamilies& GetQueueFamilies() const;

		std::weak_ptr<PhysicalDevice> GetPhysicalDevice() const;

		void CleanUp() const;

	private:
		VkDevice logicalDevice;

		std::shared_ptr<PhysicalDevice> physicalDevice;

		QueueFamilies queueFamilies;

		VkPhysicalDeviceFeatures deviceFeatures;

		friend class RenderApi;

		void InstantiatePhysicalDevicesAndQueueFamilies(const std::shared_ptr<VulkanInstanceWrapper>& vulkanInstanceWrapper,
			const std::shared_ptr<Windows::VulkanWindowSurface>& vulkanWindowSurface);

		bool CheckDeviceExtensionSupport() const;
	};
}
