#include <SculptorPch.h>

#include "QueueFamilies.h"

#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	QueueFamilies::QueueFamilies()
		:	graphicsQueue(nullptr),
			presentQueue(nullptr)
	{
	}

	void QueueFamilies::InstantiateAndFindQueueFamilies(const std::shared_ptr<PhysicalDevice>& physicalDevice,
		const std::shared_ptr<Windows::VulkanWindowSurface>& vulkanWindowSurface)
	{
		// Two step process to get Queue Families
		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice->GetPrimaryPhysicalDevice(), &queueFamilyCount, nullptr);

		queueFamilies.resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice->GetPrimaryPhysicalDevice(), &queueFamilyCount, queueFamilies.data());

		S_ASSERT(queueFamilies.empty(), "Instantiate Queue Family List before proceeding to get Graphic Family!");

		int i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				queueFamilyIndices.graphicsFamily = i;

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice->GetPrimaryPhysicalDevice(), i, vulkanWindowSurface->GetSurface(), &presentSupport);

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

	bool QueueFamilies::IsDeviceSuitable() const
	{
		return queueFamilyIndices.IsComplete();
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
