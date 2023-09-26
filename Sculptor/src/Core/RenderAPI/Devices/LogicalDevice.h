#pragma once

#include "Core/RenderAPI/ValidationLayer/ValidationLayer.h"
#include "QueueFamily/QueueFamilies.h"

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

		void CleanUp() const;

	private:
		VkDevice logicalDevice;

		std::shared_ptr<PhysicalDevice> physicalDevice;

		QueueFamilies queueFamilies;

		VkPhysicalDeviceFeatures deviceFeatures;

		void InstantiatePhysicalDevicesAndQueueFamilies(const std::shared_ptr<VulkanInstanceWrapper>& vulkanInstanceWrapper,
			const std::shared_ptr<Windows::VulkanWindowSurface>& vulkanWindowSurface);
	};
}
