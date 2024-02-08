#include <SculptorPch.h>

#include <utility>

#include "PhysicalDevice.h"

#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	PhysicalDevice::PhysicalDevice()
		:	primaryPhysicalDevice(VK_NULL_HANDLE)
	{ }

	PhysicalDevice::PhysicalDevice(const std::shared_ptr<VulkanInstanceWrapper>& vulkanInstanceWrapper)
		:	primaryPhysicalDevice(VK_NULL_HANDLE)
	{
		// NOTE: Can fetch all physical device since, now have pointer to VulkanInstanceWrapper
		FetchAllPhysicalDevicesAndPickPrimary(vulkanInstanceWrapper);
	}

	void PhysicalDevice::SetAnisotropyFeatures(bool value)
	{
		physicalDeviceFeatures.samplerAnisotropy = value;
	}

	void PhysicalDevice::FetchAllPhysicalDevicesAndPickPrimary(std::weak_ptr<VulkanInstanceWrapper> vulkanInstanceWrapper)
	{
		GetShared<VulkanInstanceWrapper> vulkanInstanceWrapperPtr{ std::move(vulkanInstanceWrapper) };
		const auto& vulkanInstance = vulkanInstanceWrapperPtr->GetInstance();

		// Get the count of available devices
		uint32_t deviceCount;
		VK_CHECK(vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, nullptr), "Failed to get device count.")
		S_ASSERT(deviceCount == 0, "Failed to find any GPUs with Vulkan Support!")

		// Put those devices in vector of VkPhysical Devices
		physicalDevicesList.resize(deviceCount);
		VK_CHECK(vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, physicalDevicesList.data()), "Failed to populate Physical Devices List.")

		PickPrimaryPhysicalDevice();
	}

	const std::vector<VkPhysicalDevice>& PhysicalDevice::GetAllDevices() const
	{
		return physicalDevicesList;
	}

	VkPhysicalDevice PhysicalDevice::GetPrimaryDevice() const
	{
		return primaryPhysicalDevice;
	}

	const VkPhysicalDeviceFeatures& PhysicalDevice::GetDeviceFeatures() const
	{
		return physicalDeviceFeatures;
	}

	void PhysicalDevice::PickPrimaryPhysicalDevice()
	{
		S_ASSERT(physicalDevicesList.empty() == true, "Trying to pick Primary Physical Device before fetching all physical devices!");

		// Choose one suitable device
		// Developer Comment: Choose the fastest one (psst, NVIDIA)
		std::multimap<int, VkPhysicalDevice> candidateDevices;
		for (const auto& device : physicalDevicesList)
		{
			int score = RateDeviceSuitability(device);
			candidateDevices.insert(std::make_pair(score, device));
		}

		// Pick the first candidate cause it's a ordered map
		if (candidateDevices.rbegin()->first > 0)
			primaryPhysicalDevice = candidateDevices.rbegin()->second;

		// If all candidates return with 0 then not compatible device found
		S_ASSERT(primaryPhysicalDevice == VK_NULL_HANDLE, "Failded to find any GPUs with Vulkan Support");
	}

	// Private/Helper Functions
	int PhysicalDevice::RateDeviceSuitability(const VkPhysicalDevice& device)
	{
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);

		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		int score = 0;

		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			score += 1000;

		score += static_cast<int>(deviceProperties.limits.maxImageDimension2D);

		if (!deviceFeatures.geometryShader)
			return 0;

		return score;
	}
}
