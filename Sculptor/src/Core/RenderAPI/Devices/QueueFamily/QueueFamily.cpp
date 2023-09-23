#include <SculptorPch.h>

#include "QueueFamily.h"

#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	QueueFamily::QueueFamily()
		:	graphicsQueue(nullptr),
			queueFamilyCount(0),
			physicalDevice(nullptr)
	{
	}

	void QueueFamily::SetPhysicalDevice(const std::shared_ptr<PhysicalDevice>& physicalDevice)
	{
		this->physicalDevice = physicalDevice;
	}

	const std::shared_ptr<PhysicalDevice>& QueueFamily::GetPhysicalDevice() const
	{
		return physicalDevice;
	}

	void QueueFamily::InstantiateQueueFamilies()
	{
		// Two step process to get Queue Families
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice->GetPrimaryPhysicalDevice(), &queueFamilyCount, nullptr);

		queueFamilies.resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice->GetPrimaryPhysicalDevice(), &queueFamilyCount, queueFamilies.data());
	}

	const std::vector<VkQueueFamilyProperties>& QueueFamily::GetQueueFamiliesList() const
	{
		S_ASSERT(queueFamilies.empty(), "Instantiate Queue Family List before retriving it!");
		return queueFamilies;
	}

	const std::vector<VkQueueFamilyProperties>& QueueFamily::InstantiateAndFetchQueueFamilies()
	{
		InstantiateQueueFamilies();

		return queueFamilies;
	}

	void QueueFamily::FindGraphicQueueFamilies()
	{
		S_ASSERT(queueFamilies.empty(), "Instantiate Queue Family List before proceeding to get Graphic Family!");

		int i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				queueFamilyIndices.graphicsFamily = i;

			// break if we found a suitable graphicsFamily
			if (queueFamilyIndices.IsGraphicsFamilyComplete())
				break;

			++i;
		}
	}

	bool QueueFamily::IsDeviceSuitable() const
	{
		return queueFamilyIndices.IsGraphicsFamilyComplete();
	}

	const QueueFamilyIndices& QueueFamily::GetQueueFamilyIndices() const
	{
		return queueFamilyIndices;
	}

	const VkQueue& QueueFamily::GetQueue() const
	{
		return graphicsQueue;
	}

	VkQueue& QueueFamily::GetQueue()
	{
		return graphicsQueue;
	}
}
