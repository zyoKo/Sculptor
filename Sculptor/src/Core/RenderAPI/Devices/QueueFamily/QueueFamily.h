#pragma once

#include "Core/RenderAPI/Devices/PhysicalDevice.h"
#include "Structures/QueueFamilyIndices.h"

namespace Sculptor::Core
{
	class QueueFamily
	{
	public:
		QueueFamily();

		~QueueFamily() = default;

		void SetPhysicalDevice(const std::shared_ptr<PhysicalDevice>& physicalDevice);

		const std::shared_ptr<PhysicalDevice>& GetPhysicalDevice() const;

		void InstantiateQueueFamilies();

		const std::vector<VkQueueFamilyProperties>& GetQueueFamiliesList() const;

		const std::vector<VkQueueFamilyProperties>& InstantiateAndFetchQueueFamilies();

		// TODO: Extend it to support multiple Queue Families
		void FindGraphicQueueFamilies();

		bool IsDeviceSuitable() const;

		const QueueFamilyIndices& GetQueueFamilyIndices() const;

		const VkQueue& GetQueue() const;

		VkQueue& GetQueue();

	private:
		VkQueue graphicsQueue;

		QueueFamilyIndices queueFamilyIndices;

		uint32_t queueFamilyCount;

		std::vector<VkQueueFamilyProperties> queueFamilies;

		std::shared_ptr<PhysicalDevice> physicalDevice;
	};
}
