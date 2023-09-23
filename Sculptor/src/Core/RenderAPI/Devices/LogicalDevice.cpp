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

	void LogicalDevice::InstantiatePhysicalDevice() const
	{
		physicalDevice->SetVulkanInstanceWrapper(vulkanInstanceWrapper);

		physicalDevice->FetchAllPhysicalDevicesAndPickPrimary();
	}

	void LogicalDevice::InstantiateQueueFamily()
	{
		queueFamily.SetPhysicalDevice(physicalDevice);
		queueFamily.InstantiateQueueFamilies();
		queueFamily.FindGraphicQueueFamilies();
		S_ASSERT(!queueFamily.IsDeviceSuitable(), "Failed to initialize graphics family!");
	}

	void LogicalDevice::CreateLogicalDevice()
	{
		InstantiatePhysicalDevice();

		InstantiateQueueFamily();

		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily.GetQueueFamilyIndices().graphicsFamily.value();  // NOLINT(bugprone-unchecked-optional-access): already checking in the ASSERT above
		queueCreateInfo.queueCount = 1;

		// required to setup queuePriority because Vulkan demands it even if there is only single queue
		constexpr float queuePriority = 1.0f;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = &queueCreateInfo;
		createInfo.queueCreateInfoCount = 1;
		createInfo.pEnabledFeatures = &deviceFeatures;

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (validationLayer && validationLayer->IsEnabled()) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayer->GetValidationLayersArray().size());
			createInfo.ppEnabledLayerNames = validationLayer->GetValidationLayersArray().data();

			validationLayer->PopulateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = &debugCreateInfo;
		}
		else {
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		const auto result = vkCreateDevice(queueFamily.GetPhysicalDevice()->GetPrimaryPhysicalDevice(), &createInfo, nullptr, &logicalDevice);
		S_ASSERT(result != VK_SUCCESS, "Failed to create Logical Device!");

		vkGetDeviceQueue(
			logicalDevice, 
			queueFamily.GetQueueFamilyIndices().graphicsFamily.value(), // NOLINT(bugprone-unchecked-optional-access): already checking in the ASSERT above
			0, 
			&queueFamily.GetQueue());
	}

	void LogicalDevice::CleanUp() const
	{
		vkDestroyDevice(logicalDevice, nullptr);
	}
}
