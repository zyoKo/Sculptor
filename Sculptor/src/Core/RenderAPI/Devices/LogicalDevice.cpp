#include <SculptorPch.h>

#include "LogicalDevice.h"

#include "Core/RenderAPI/RenderApi.h"
#include "Core/RenderAPI/Data/Constants.h"
#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	LogicalDevice::LogicalDevice()
		:	logicalDevice(nullptr),
			physicalDevice(std::make_shared<PhysicalDevice>()),
			deviceFeatures{}
	{ }

	void LogicalDevice::CreateLogicalDevice(
		const std::shared_ptr<VulkanInstanceWrapper>& vulkanInstanceWrapper,
		const std::shared_ptr<ValidationLayer>& validationLayer, 
		const std::shared_ptr<Windows::VulkanWindowSurface>& vulkanWindowSurface)
	{
		// -- Big Step here --
		// Physical Devices and Queue Families Initialization
		InstantiatePhysicalDevicesAndQueueFamilies(vulkanInstanceWrapper, vulkanWindowSurface);

		const bool allGood = RenderApi::IsDeviceSuitable(*this, vulkanWindowSurface);
		std::cout << "Can Create Logical Device?: " << std::boolalpha << allGood << std::endl;

		// Queue Family Indices
		const auto& indices = queueFamilies.GetQueueFamilyIndices();

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfoList;
		const std::set<uint32_t> uniqueQueueFamilies = {
			indices.graphicsFamily.value(),
			indices.presetFamily.value()
		};

		// required to setup queuePriority because Vulkan demands it even if there is only single queue
		constexpr float queuePriority = 1.0f;
		for (const auto queueFamily : uniqueQueueFamilies)
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

		createInfo.enabledExtensionCount = static_cast<uint32_t>(DEVICE_EXTENSIONS.size());
		createInfo.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();

		if (validationLayer && validationLayer->IsEnabled()) 
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayer->GetValidationLayersArray().size());
			createInfo.ppEnabledLayerNames = validationLayer->GetValidationLayersArray().data();
		}
		else 
		{
			createInfo.enabledLayerCount = 0;
		}

		createInfo.pNext = nullptr;

		const auto result = vkCreateDevice(physicalDevice->GetPrimaryPhysicalDevice(), &createInfo, nullptr, &logicalDevice);
		S_ASSERT(result != VK_SUCCESS, "Failed to create Logical Device!");

		// Graphics-Queue for Logical Device
		vkGetDeviceQueue(
			logicalDevice, 
			indices.graphicsFamily.value(),
			0, 
			&queueFamilies.GetGraphicsQueue());

		// Present-Queue for Logical Device
		vkGetDeviceQueue(
			logicalDevice,
			indices.presetFamily.value(),
			0,
			&queueFamilies.GetPresentQueue());
	}

	const VkDevice& LogicalDevice::GetLogicalDevice() const
	{
		return logicalDevice;
	}

	const QueueFamilies& LogicalDevice::GetQueueFamilies() const
	{
		return queueFamilies;
	}

	std::weak_ptr<PhysicalDevice> LogicalDevice::GetPhysicalDevice() const
	{
		return physicalDevice;
	}

	void LogicalDevice::CleanUp() const
	{
		vkDestroyDevice(logicalDevice, nullptr);
	}

	void LogicalDevice::InstantiatePhysicalDevicesAndQueueFamilies(const std::shared_ptr<VulkanInstanceWrapper>& vulkanInstanceWrapper,
		const std::shared_ptr<Windows::VulkanWindowSurface>& vulkanWindowSurface)
	{
		physicalDevice->FetchAllPhysicalDevicesAndPickPrimary(vulkanInstanceWrapper);

		queueFamilies.InstantiateAndFindQueueFamilies(physicalDevice, vulkanWindowSurface);
	}

	bool LogicalDevice::CheckDeviceExtensionSupport() const
	{
		const auto& device = physicalDevice->GetPrimaryPhysicalDevice();

		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(DEVICE_EXTENSIONS.begin(), DEVICE_EXTENSIONS.end());

		for (const auto& extensions : availableExtensions)
		{
			requiredExtensions.erase(extensions.extensionName);
		}

		return requiredExtensions.empty();
	}
}
