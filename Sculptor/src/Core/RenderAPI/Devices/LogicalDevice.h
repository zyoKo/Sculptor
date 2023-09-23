#pragma once

#include "Core/RenderAPI/ValidationLayer/ValidationLayer.h"
#include "QueueFamily/QueueFamily.h"

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

		void InstantiatePhysicalDevice() const;

		void InstantiateQueueFamily();

		void CreateLogicalDevice();

		void CleanUp() const;

	private:
		VkDevice logicalDevice;

		std::shared_ptr<PhysicalDevice> physicalDevice;

		QueueFamily queueFamily;

		VkPhysicalDeviceFeatures deviceFeatures;

		std::shared_ptr<ValidationLayer> validationLayer;

		std::shared_ptr<VulkanInstanceWrapper> vulkanInstanceWrapper;
	};
}
