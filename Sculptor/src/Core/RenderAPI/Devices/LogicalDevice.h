#pragma once

#include "Core/RenderAPI/ValidationLayer/ValidationLayer.h"
#include "QueueFamily/QueueFamilies.h"

namespace Sculptor::Core
{
	class SwapChain;
}

namespace Sculptor::Core
{
	class LogicalDevice
	{
	public:
		LogicalDevice();

		~LogicalDevice() = default;

		void Create(std::weak_ptr<VulkanInstanceWrapper> instance, std::weak_ptr<ValidationLayer> validationLayer, std::weak_ptr<Windows::VulkanWindowSurface> surface);

		void SetAnisotropyFeatures(bool value) const;

		VkDevice Get() const;

		const QueueFamilies& GetQueueFamilies() const;

		std::weak_ptr<PhysicalDevice> GetPhysicalDevice() const;

		void CleanUp() const;

	private:
		VkDevice logicalDevice;

		std::shared_ptr<PhysicalDevice> physicalDevice;

		QueueFamilies queueFamilies;

		friend class RenderApi;

		void InstantiatePhysicalDevicesAndQueueFamilies(const std::weak_ptr<VulkanInstanceWrapper>& vulkanInstanceWrapper,
			const std::weak_ptr<Windows::VulkanWindowSurface>& vulkanWindowSurface);

		bool CheckDeviceExtensionSupport() const;
	};
}
