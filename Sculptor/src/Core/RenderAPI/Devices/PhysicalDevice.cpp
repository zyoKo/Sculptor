#include <SculptorPch.h>

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

	void PhysicalDevice::FetchAllPhysicalDevicesAndPickPrimary(const std::shared_ptr<VulkanInstanceWrapper>& vulkanInstanceWrapper)
	{
		// Get the count of available devices
		uint32_t deviceCount;
		vkEnumeratePhysicalDevices(vulkanInstanceWrapper->GetInstance(), &deviceCount, nullptr);
		S_ASSERT(deviceCount == 0, "Failed to find any GPUs with Vulkan Support!");

		// Put those devices in vector of VkPhysical Devices
		physicalDevicesList.resize(deviceCount);
		vkEnumeratePhysicalDevices(vulkanInstanceWrapper->GetInstance(), &deviceCount, physicalDevicesList.data());

		PickPrimaryPhysicalDevice();
	}

	const std::vector<VkPhysicalDevice>& PhysicalDevice::GetAllPhysicalDevices()
	{
		return physicalDevicesList;
	}

	const VkPhysicalDevice& PhysicalDevice::GetPrimaryPhysicalDevice() const
	{
		S_ASSERT(primaryPhysicalDevice == VK_NULL_HANDLE, "Fetching Primary PhysicalDevice before instantiating it!");
		return primaryPhysicalDevice;
	}

	VkPhysicalDevice& PhysicalDevice::GetPrimaryPhysicalDevice()
	{
		return primaryPhysicalDevice;
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
