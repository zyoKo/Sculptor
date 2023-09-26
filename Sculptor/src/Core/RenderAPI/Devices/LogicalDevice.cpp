#include <SculptorPch.h>

#include "LogicalDevice.h"

#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	LogicalDevice::LogicalDevice()
		:	logicalDevice(nullptr),
			physicalDevice(std::make_shared<PhysicalDevice>()),
			deviceFeatures{}
	{
	}

	LogicalDevice::LogicalDevice(const std::shared_ptr<ValidationLayer>& validationLayer, const std::shared_ptr<VulkanInstanceWrapper>& vulkanInstanceWrap)
		:	logicalDevice(nullptr),
			physicalDevice(std::make_shared<PhysicalDevice>()),
			deviceFeatures{},
			validationLayer(validationLayer),
			vulkanInstanceWrapper(vulkanInstanceWrap)
	{
	}

	void LogicalDevice::SetValidationLayer(const std::shared_ptr<ValidationLayer>& validationLayer)
	{
		this->validationLayer = validationLayer;
	}

	const std::shared_ptr<ValidationLayer>& LogicalDevice::GetValidationLayer() const
	{
		return validationLayer;
	}

	void LogicalDevice::SetVulkanInstanceWrapper(const std::shared_ptr<VulkanInstanceWrapper>& vulkanInstanceWrap)
	{
		this->vulkanInstanceWrapper = vulkanInstanceWrap;
	}

	const std::shared_ptr<VulkanInstanceWrapper>& LogicalDevice::GetVulkanInstanceWrapper() const
	{
		return vulkanInstanceWrapper;
	}

	void LogicalDevice::SetVulkanWindowSurface(const std::shared_ptr<Windows::VulkanWindowSurface>& vulkanWindowSurface)
	{
		this->vulkanWindowSurface = vulkanWindowSurface;
	}

	const std::shared_ptr<Windows::VulkanWindowSurface>& LogicalDevice::GetVulkanWindowSurface() const
	{
		return vulkanWindowSurface;
	}

	void LogicalDevice::InstantiatePhysicalDevice() const
	{
		physicalDevice->SetVulkanInstanceWrapper(vulkanInstanceWrapper);

		physicalDevice->FetchAllPhysicalDevicesAndPickPrimary();
	}

	void LogicalDevice::InstantiateQueueFamily()
	{
		queueFamilies.SetPhysicalDevice(physicalDevice);

		queueFamilies.SetVulkanWindowSurface(vulkanWindowSurface);

		queueFamilies.InstantiateQueueFamilies();

		queueFamilies.FindQueueFamilies();

		S_ASSERT(!queueFamilies.IsDeviceSuitable(), "Failed to initialize queue families!");
	}

	void LogicalDevice::CreateLogicalDevice()
	{
		InstantiatePhysicalDevice();

		InstantiateQueueFamily();

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfoList;
		const std::set<uint32_t> uniqueQueueFamilies = {
			queueFamilies.GetQueueFamilyIndices().graphicsFamily.value(),
			queueFamilies.GetQueueFamilyIndices().presetFamily.value()
		};

		// required to setup queuePriority because Vulkan demands it even if there is only single queue
		constexpr float queuePriority = 1.0f;
		for (const uint32_t queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;

			// push_back the QueueInfo to the vector
			queueCreateInfoList.push_back(queueCreateInfo);
		}

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfoList.size());
		createInfo.pQueueCreateInfos = queueCreateInfoList.data();
		createInfo.pEnabledFeatures = &deviceFeatures;

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (validationLayer && validationLayer->IsEnabled()) 
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayer->GetValidationLayersArray().size());
			createInfo.ppEnabledLayerNames = validationLayer->GetValidationLayersArray().data();

			validationLayer->PopulateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = &debugCreateInfo;
		}
		else 
		{
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		const auto result = vkCreateDevice(queueFamilies.GetPhysicalDevice()->GetPrimaryPhysicalDevice(), &createInfo, nullptr, &logicalDevice);
		S_ASSERT(result != VK_SUCCESS, "Failed to create Logical Device!");

		// Graphics-Queue for Logical Device
		vkGetDeviceQueue(
			logicalDevice, 
			queueFamilies.GetQueueFamilyIndices().graphicsFamily.value(),
			0, 
			&queueFamilies.GetGraphicsQueue());

		// Present-Queue for Logical Device
		vkGetDeviceQueue(
			logicalDevice,
			queueFamilies.GetQueueFamilyIndices().presetFamily.value(),
			0,
			&queueFamilies.GetPresentQueue());
	}

	void LogicalDevice::CleanUp() const
	{
		vkDestroyDevice(logicalDevice, nullptr);
	}
}
