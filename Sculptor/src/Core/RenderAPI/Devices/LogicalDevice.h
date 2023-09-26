#pragma once

#include "Core/RenderAPI/ValidationLayer/ValidationLayer.h"
#include "QueueFamily/QueueFamilies.h"

namespace Sculptor::Core
{
	class LogicalDevice
	{
	public:
		LogicalDevice();

		LogicalDevice(const std::shared_ptr<ValidationLayer>& validationLayer, const std::shared_ptr<VulkanInstanceWrapper>& vulkanInstanceWrap);

		~LogicalDevice() = default;

		void SetValidationLayer(const std::shared_ptr<ValidationLayer>& validationLayer);

		const std::shared_ptr<ValidationLayer>& GetValidationLayer() const;

		void SetVulkanInstanceWrapper(const std::shared_ptr<VulkanInstanceWrapper>& vulkanInstanceWrap);

		const std::shared_ptr<VulkanInstanceWrapper>& GetVulkanInstanceWrapper() const;

		void SetVulkanWindowSurface(const std::shared_ptr<Windows::VulkanWindowSurface>& vulkanWindowSurface);

		const std::shared_ptr<Windows::VulkanWindowSurface>& GetVulkanWindowSurface() const;

		void InstantiatePhysicalDevice() const;

		void InstantiateQueueFamily();

		void CreateLogicalDevice();

		void CleanUp() const;

	private:
		VkDevice logicalDevice;

		std::shared_ptr<PhysicalDevice> physicalDevice;

		QueueFamilies queueFamilies;

		VkPhysicalDeviceFeatures deviceFeatures;

		std::shared_ptr<ValidationLayer> validationLayer;

		std::shared_ptr<VulkanInstanceWrapper> vulkanInstanceWrapper;

		std::shared_ptr<Windows::VulkanWindowSurface> vulkanWindowSurface;
	};
}
