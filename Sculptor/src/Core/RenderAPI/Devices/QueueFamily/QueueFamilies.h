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

		void SetPhysicalDevice(const std::shared_ptr<PhysicalDevice>& physicalDevice);

		const std::shared_ptr<PhysicalDevice>& GetPhysicalDevice() const;

		void SetVulkanWindowSurface(const std::shared_ptr<Windows::VulkanWindowSurface>& vulkanWindowSurface);

		const std::shared_ptr<Windows::VulkanWindowSurface>& GetVulkanWindowSurface() const;

		void InstantiateQueueFamilies();

		const std::vector<VkQueueFamilyProperties>& GetQueueFamiliesList() const;

		const std::vector<VkQueueFamilyProperties>& InstantiateAndFetchQueueFamilies();

		void FindQueueFamilies();

		bool IsDeviceSuitable() const;

		const QueueFamilyIndices& GetQueueFamilyIndices() const;

		const VkQueue& GetGraphicsQueue() const;

		VkQueue& GetGraphicsQueue();

		const VkQueue& GetPresentQueue() const;

		VkQueue& GetPresentQueue();

	private:
		VkQueue graphicsQueue;

		VkQueue presentQueue;

		QueueFamilyIndices queueFamilyIndices;

		uint32_t queueFamilyCount;

		std::vector<VkQueueFamilyProperties> queueFamilies;

		std::shared_ptr<PhysicalDevice> physicalDevice;

		std::shared_ptr<Windows::VulkanWindowSurface> vulkanWindowSurface;
	};
}
