#include <SculptorPch.h>

#include "QueueFamilies.h"

#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	QueueFamilies::QueueFamilies()
		:	graphicsQueue(nullptr),
			presentQueue(nullptr),
			queueFamilyCount(0),
			physicalDevice(nullptr)
	{
	}

	void QueueFamilies::SetPhysicalDevice(const std::shared_ptr<PhysicalDevice>& physicalDevice)
	{
		this->physicalDevice = physicalDevice;
	}

	const std::shared_ptr<PhysicalDevice>& QueueFamilies::GetPhysicalDevice() const
	{
		return physicalDevice;
	}

	void QueueFamilies::SetVulkanWindowSurface(const std::shared_ptr<Windows::VulkanWindowSurface>& vulkanWindowSurface)
	{
		this->vulkanWindowSurface = vulkanWindowSurface;
	}

	const std::shared_ptr<Windows::VulkanWindowSurface>& QueueFamilies::GetVulkanWindowSurface() const
	{
		return vulkanWindowSurface;
	}

	void QueueFamilies::InstantiateQueueFamilies()
	{
		// Two step process to get Queue Families
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice->GetPrimaryPhysicalDevice(), &queueFamilyCount, nullptr);

		queueFamilies.resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice->GetPrimaryPhysicalDevice(), &queueFamilyCount, queueFamilies.data());
	}

	const std::vector<VkQueueFamilyProperties>& QueueFamilies::GetQueueFamiliesList() const
	{
		S_ASSERT(queueFamilies.empty(), "Instantiate Queue Family List before retriving it!");
		return queueFamilies;
	}

	const std::vector<VkQueueFamilyProperties>& QueueFamilies::InstantiateAndFetchQueueFamilies()
	{
		InstantiateQueueFamilies();

		return queueFamilies;
	}

	void QueueFamilies::FindQueueFamilies()
	{
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

	bool QueueFamilies::IsDeviceSuitable() const
	{
		return queueFamilyIndices.IsComplete();
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
