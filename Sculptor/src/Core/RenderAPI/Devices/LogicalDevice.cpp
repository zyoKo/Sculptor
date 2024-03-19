#include <SculptorPch.h>

#include "LogicalDevice.h"

#include "Core/RenderAPI/RenderApi.h"
#include "Core/Data/Constants.h"
#include "Core/RenderAPI/Utility/CreateInfo.h"
#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	LogicalDevice::LogicalDevice()
		:	logicalDevice(VK_NULL_HANDLE),
			physicalDevice(std::make_shared<PhysicalDevice>())
	{
		// Enable Anisotropy
		physicalDevice->SetAnisotropyFeatures(true);
	}

	void LogicalDevice::Create(std::weak_ptr<VulkanInstanceWrapper> instance, std::weak_ptr<ValidationLayer> validationLayer, std::weak_ptr<Windows::VulkanWindowSurface> surface)
	{
		// -- Big Step here --
		// Physical Devices and Queue Families Initialization
		InstantiatePhysicalDevicesAndQueueFamilies(instance, surface);

		S_ASSERT(!RenderApi::IsDeviceSuitable(*this, surface), "No suitable Device found with all requested features.")

		// Queue Family Indices
		const auto& indices = queueFamilies.GetQueueFamilyIndices();

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfoList;
		const std::set<uint32_t> uniqueQueueFamilies = {
			indices.GetGraphicsFamily().value(),
			indices.GetPresetFamily().value()
		};

		// required to set up queuePriority because Vulkan demands it even if there is only single queue
		constexpr float queuePriority = 1.0f;
		for (const auto queueFamily : uniqueQueueFamilies)
		{
			const auto queueCreateInfo = CreateInfo<VkDeviceQueueCreateInfo>({
				.queueFamilyIndex = queueFamily,
				.queueCount = 1,
				.pQueuePriorities = &queuePriority
			});

			// push_back the QueueInfo to the vector
			queueCreateInfoList.push_back(queueCreateInfo);
		}

		auto createInfo = CreateInfo<VkDeviceCreateInfo>({
			.queueCreateInfoCount		= static_cast<uint32_t>(queueCreateInfoList.size()),
			.pQueueCreateInfos			= queueCreateInfoList.data(),
			.enabledExtensionCount		= static_cast<uint32_t>(DEVICE_EXTENSIONS.size()),
			.ppEnabledExtensionNames	= DEVICE_EXTENSIONS.data(),
			.pEnabledFeatures			= &physicalDevice->GetDeviceFeatures()
		});

		GetShared<ValidationLayer> validationLayerPtr { std::move(validationLayer) };

		if (validationLayerPtr != nullptr && validationLayerPtr->IsEnabled())
		{
			createInfo.enabledLayerCount	= static_cast<uint32_t>(validationLayerPtr->GetValidationLayersArray().size());
			createInfo.ppEnabledLayerNames	= validationLayerPtr->GetValidationLayersArray().data();
		}
		else 
		{
			createInfo.enabledLayerCount = 0;
		}

		VK_CHECK(vkCreateDevice(physicalDevice->GetPrimaryDevice(), &createInfo, nullptr, &logicalDevice), "Failed to create Logical Device!")

		// Graphics-Queue for Logical Device
		vkGetDeviceQueue(
			logicalDevice, 
			indices.GetGraphicsFamily().value(),
			0, 
			&queueFamilies.GetGraphicsQueue());

		// Present-Queue for Logical Device
		vkGetDeviceQueue(
			logicalDevice,
			indices.GetPresetFamily().value(),
			0,
			&queueFamilies.GetPresentQueue());
	}

	void LogicalDevice::SetAnisotropyFeatures(bool value) const
	{
		physicalDevice->SetAnisotropyFeatures(value);
	}

	VkDevice LogicalDevice::Get() const
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

	void LogicalDevice::InstantiatePhysicalDevicesAndQueueFamilies(const std::weak_ptr<VulkanInstanceWrapper>& vulkanInstanceWrapper,
		const std::weak_ptr<Windows::VulkanWindowSurface>& vulkanWindowSurface)
	{
		physicalDevice->FetchAllPhysicalDevicesAndPickPrimary(vulkanInstanceWrapper);

		queueFamilies.InstantiateAndFindQueueFamilies(physicalDevice, vulkanWindowSurface);
	}

	bool LogicalDevice::CheckDeviceExtensionSupport() const
	{
		const auto& device = physicalDevice->GetPrimaryDevice();

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
