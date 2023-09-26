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

		void FetchAllPhysicalDevicesAndPickPrimary(const std::shared_ptr<VulkanInstanceWrapper>& vulkanInstanceWrapper);

		const std::vector<VkPhysicalDevice>& GetAllPhysicalDevices();

		const VkPhysicalDevice& GetPrimaryPhysicalDevice() const;

		VkPhysicalDevice& GetPrimaryPhysicalDevice();

	private:
		VkPhysicalDevice primaryPhysicalDevice;

		std::vector<VkPhysicalDevice> physicalDevicesList;

		void PickPrimaryPhysicalDevice();

		static int RateDeviceSuitability(const VkPhysicalDevice& device);
	};
}
