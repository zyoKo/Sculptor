#pragma once

#include "Core/RenderAPI/VulkanInstance/VulkanInstanceWrapper.h"

namespace Sculptor::Core
{
	class PhysicalDevice
	{
	public:
		PhysicalDevice();

		PhysicalDevice(const std::shared_ptr<VulkanInstanceWrapper>& vulkanInstanceWrapper);

		~PhysicalDevice() = default;

		void SetAnisotropyFeatures(bool value);

		void FetchAllPhysicalDevicesAndPickPrimary(std::weak_ptr<VulkanInstanceWrapper> vulkanInstanceWrapper);

		const std::vector<VkPhysicalDevice>& GetAllDevices() const;

		VkPhysicalDevice GetPrimaryDevice() const;

		const VkPhysicalDeviceFeatures& GetDeviceFeatures() const;

	private:
		VkPhysicalDevice primaryPhysicalDevice;

		std::vector<VkPhysicalDevice> physicalDevicesList;

		VkPhysicalDeviceFeatures physicalDeviceFeatures{};

		void PickPrimaryPhysicalDevice();

		static int RateDeviceSuitability(const VkPhysicalDevice& device);
	};
}
