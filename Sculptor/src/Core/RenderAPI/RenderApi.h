#pragma once
#include "Devices/LogicalDevice.h"
#include "Devices/QueueFamily/QueueFamilies.h"
#include "SwapChains/SwapChains.h"

namespace Sculptor::Core
{
	class RenderApi
	{
	public:
		static bool IsDeviceSuitable(const LogicalDevice& device, 
			const std::weak_ptr<Windows::VulkanWindowSurface>& vulkanWindowSurface)
		{
			const auto& queueFamilies = device.GetQueueFamilies();
			const bool isDeviceSuitable = queueFamilies.GetQueueFamilyIndices().IsComplete();

			const auto physicalDevice = device.GetPhysicalDevice().lock();
			if (!physicalDevice)
			{
				std::cerr << "Initialize Physical Device before checking device suitability." << std::endl;
				return false;
			}

			const bool checkDeviceExtensionSupport = device.CheckDeviceExtensionSupport();

			bool swapChainAdequate = false;
			if (checkDeviceExtensionSupport)
			{
				const auto swapChainSupportDetails = SwapChains::QuerySwapChainSupport(vulkanWindowSurface, device.GetPhysicalDevice());
				swapChainAdequate = !swapChainSupportDetails.formats.empty() && !swapChainSupportDetails.presentModes.empty();
			}

			return isDeviceSuitable && checkDeviceExtensionSupport && swapChainAdequate;
		}
	};
}
