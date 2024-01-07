#pragma once

#include "Core/RenderAPI/Devices/PhysicalDevice.h"
#include "Core/Windows/VulkanWindowSurface/VulkanWindowSurface.h"
#include "Structures/QueueFamilyIndices.h"

namespace Sculptor::Core
{
	class QueueFamilies
	{
	public:
		QueueFamilies();

		~QueueFamilies() = default;

		void InstantiateAndFindQueueFamilies(const std::shared_ptr<PhysicalDevice>& physicalDevice,
			const std::shared_ptr<Windows::VulkanWindowSurface>& vulkanWindowSurface);

		const std::vector<VkQueueFamilyProperties>& GetQueueFamiliesList() const;

		const QueueFamilyIndices& GetQueueFamilyIndices() const;

		const VkQueue& GetGraphicsQueue() const;

		VkQueue& GetGraphicsQueue();

		const VkQueue& GetPresentQueue() const;

		VkQueue& GetPresentQueue();

	private:
		VkQueue graphicsQueue;

		VkQueue presentQueue;

		QueueFamilyIndices queueFamilyIndices;

		std::vector<VkQueueFamilyProperties> queueFamilies;
	};
}
