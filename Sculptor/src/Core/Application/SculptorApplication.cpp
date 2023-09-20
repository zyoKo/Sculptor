#include <SculptorPch.h>

#include "SculptorApplication.h"

#include "Utilities/Logger/Assert.h"
#include "Core/RenderAPI/ExtensionManager.h"

namespace Sculptor::Core
{
	SculptorApplication::SculptorApplication()
		:	window(std::make_unique<Window>()),
			windowProperties(1920, 1080),
			vulkanInstanceWrapper(std::make_shared<VulkanInstanceWrapper>()),
			validationLayer(std::make_shared<ValidationLayer>())
	{
		ExtensionManager::Initialize(validationLayer);
	}

	void SculptorApplication::Sculpt()
	{
		InitializeWindow();

		InitializeVulkan();

		MainLoop();

		CleanUp();
	}

	void SculptorApplication::InitializeWindow() const
	{
		window->InitializeWindow(windowProperties);
	}

	void SculptorApplication::InitializeVulkan()
	{
		validationLayer->RequestValidationLayer(&vulkanInstanceWrapper->GetInstance());

		vulkanInstanceWrapper->CreateInstance(validationLayer);
		//vulkanInstanceWrapper->CreateInstance(nullptr);

		validationLayer->SetupDebugMessenger();

		PickPhysicalDevice();

		CreateLogicalDevice();
	}

	void SculptorApplication::MainLoop() const
	{
		while (!window->WindowShouldClose())
		{
			window->PollEvents();
		}
	}

	void SculptorApplication::CleanUp() const
	{
		// Cleanup logical Device Creation
		vkDestroyDevice(device, nullptr);

		validationLayer->CleanUp();

		vulkanInstanceWrapper->DestroyInstance();

		window->Shutdown();
	}

	void SculptorApplication::PickPhysicalDevice()
	{
		// No cleanup functions required for VkPhysicalDevice
		// Implicitly deleted with VkInstance
		// Developer Comment: NICE
		physicalDevice = VK_NULL_HANDLE;

		// Two step method to get all the physical devices
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(vulkanInstanceWrapper->GetInstance(), &deviceCount, nullptr);
		S_ASSERT(deviceCount == 0, "Failed to find any GPUs with Vulkan Support!");
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(vulkanInstanceWrapper->GetInstance(), &deviceCount, devices.data());

		// Choose one suitable device
		// Developer Comment: Choose the fastest one (psst, NVIDIA)
		std::multimap<int, VkPhysicalDevice> candidateDevices;
		for (const auto& device : devices)
		{
			int score = RateDeviceSuitability(device);
			candidateDevices.insert(std::make_pair(score, device));
		}

		// Pick the first candidate cause it's a ordered map
		if (candidateDevices.rbegin()->first > 0)
			physicalDevice = candidateDevices.rbegin()->second;

		// If all candidates return with 0 then not compatible device found
		S_ASSERT(physicalDevice == VK_NULL_HANDLE, "Failded to find any GPUs with Vulkan Support");
	}

	int SculptorApplication::RateDeviceSuitability(const VkPhysicalDevice& device)
	{
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);

		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		int score = 0;

		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			score += 1000;

		score += static_cast<int>(deviceProperties.limits.maxImageDimension2D);

		if (!deviceFeatures.geometryShader)
			return 0;

		return score;
	}

	QueueFamilyIndices SculptorApplication::FindQueueFamilies(const VkPhysicalDevice& device)
	{
		QueueFamilyIndices indices;

		// Two step process to get Queue Families
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				indices.graphicsFamily = i;

			// break if we found a suitable graphicsFamily
			if (indices.IsGraphicsFamilyComplete())
				break;

			++i;
		}

		return indices;
	}

	bool SculptorApplication::IsDeviceSuitable(const VkPhysicalDevice& device)
	{
		const QueueFamilyIndices indices = FindQueueFamilies(device);

		return indices.IsGraphicsFamilyComplete();
	}

	void SculptorApplication::CreateLogicalDevice()
	{
		QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);
		S_ASSERT(!indices.IsGraphicsFamilyComplete(), "Failed to initialize graphics family!");

		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
		queueCreateInfo.queueCount = 1;

		// required to setup queuePriority because Vulkan demands it even if there is only single queue
		constexpr float queuePriority = 1.0f;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		VkPhysicalDeviceFeatures deviceFeatures{};

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

		const auto result = vkCreateDevice(physicalDevice, &createInfo, nullptr, &device);
		S_ASSERT(result != VK_SUCCESS, "Failed to create Logical Device!");

		vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
	}
}
