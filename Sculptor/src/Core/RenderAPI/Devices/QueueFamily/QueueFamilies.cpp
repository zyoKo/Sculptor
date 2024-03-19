#include <SculptorPch.h>

#include "QueueFamilies.h"

#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	QueueFamilies::QueueFamilies()
		:	graphicsQueue(VK_NULL_HANDLE),
			presentQueue(VK_NULL_HANDLE)
	{ }

	void QueueFamilies::InstantiateAndFindQueueFamilies(std::weak_ptr<PhysicalDevice> weakPhysicalDevice,
		std::weak_ptr<Windows::VulkanWindowSurface> weakWindowSurface)
	{
		GetShared<PhysicalDevice> physicalDevicePtr{ std::move(weakPhysicalDevice) };
		const auto physicalDevice = physicalDevicePtr->GetPrimaryDevice();

		// Two-step process to get Queue Families
		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

		queueFamilies.resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

		S_ASSERT(queueFamilies.empty(), "Instantiate Queue Family List before proceeding to get Graphic Family!")

		GetShared<Windows::VulkanWindowSurface> vulkanWindowSurfacePtr{ std::move(weakWindowSurface) };
		const auto windowSurface = vulkanWindowSurfacePtr->GetSurface();

		int i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				queueFamilyIndices.graphicsFamily = i;

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, windowSurface, &presentSupport);

			if (presentSupport)
				queueFamilyIndices.presetFamily = i;

			// break if we found a suitable graphicsFamily
			if (queueFamilyIndices.IsComplete())
				break;

			++i;
		}
	}

	const std::vector<VkQueueFamilyProperties>& QueueFamilies::GetQueueFamiliesList() const
	{
		S_ASSERT(queueFamilies.empty(), "Instantiate Queue Family List before retriving it!");
		return queueFamilies;
	}

	const QueueFamilyIndices& QueueFamilies::GetQueueFamilyIndices() const
	{
		return queueFamilyIndices;
	}

	const VkQueue& QueueFamilies::GetGraphicsQueue() const
	{
		return graphicsQueue;
	}

	VkQueue& QueueFamilies::GetGraphicsQueue()
	{
		return graphicsQueue;
	}

	const VkQueue& QueueFamilies::GetPresentQueue() const
	{
		return presentQueue;
	}

	VkQueue& QueueFamilies::GetPresentQueue()
	{
		return presentQueue;
	}
}
