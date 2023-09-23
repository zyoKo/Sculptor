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

		void SetVulkanInstanceWrapper(const std::shared_ptr<VulkanInstanceWrapper>& vulkanInstanceWrap);

		const std::shared_ptr<VulkanInstanceWrapper>& GetVulkanInstanceWrapper() const;

		void FetchAllPhysicalDevicesAndPickPrimary();

		void FetchAllPhysicalDevices();

		void PickPrimaryPhysicalDevice();

		const std::vector<VkPhysicalDevice>& GetAllPhysicalDevices();

		uint32_t GetDeviceCount() const;

		const VkPhysicalDevice& GetPrimaryPhysicalDevice() const;

		VkPhysicalDevice& GetPrimaryPhysicalDevice();

	private:
		VkPhysicalDevice primaryPhysicalDevice;

		uint32_t deviceCount;

		std::vector<VkPhysicalDevice> physicalDevicesList;

		std::shared_ptr<VulkanInstanceWrapper> vulkanInstanceWrapper;

		static int RateDeviceSuitability(const VkPhysicalDevice& device);
	};
}
