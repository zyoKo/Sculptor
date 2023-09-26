#include <SculptorPch.h>

#include "GLFW/glfw3.h"

#include "VulkanInstanceWrapper.h"

#include "Utilities/ExtensionManager.h"
#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	VulkanInstanceWrapper::VulkanInstanceWrapper()
		:	vulkanInstance(nullptr)
	{
	}

	void VulkanInstanceWrapper::CreateInstance(const std::shared_ptr<ValidationLayer>& validationLayer)
	{
		S_ASSERT(!validationLayer->RequestValidationLayer(), "Failed to request validation Layer!");

		// Create Application Info
		VkApplicationInfo applicationInfo{};
		applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		applicationInfo.pApplicationName = "Sculptor Triangle";
		applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.pEngineName = "Sculptor-Engine";
		applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.apiVersion = VK_MAKE_VERSION(1, 3, 0);

		// Create Info needs both applicationInfo and glfwExtensions
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &applicationInfo;

		// Get GLFW Extension Count and Required Extensions
		const auto requiredExtensions = Utils::ExtensionManager::GetRequiredExtensions();

		createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
		createInfo.ppEnabledExtensionNames = requiredExtensions.data();

		createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

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

		// Finally create VulkanInstanceWrapper
		const VkResult result = vkCreateInstance(&createInfo, nullptr, &vulkanInstance);
		S_ASSERT(result != VK_SUCCESS, "Failed to Create Vulkan Instance!");
	}

	void VulkanInstanceWrapper::DestroyInstance() const
	{
		vkDestroyInstance(vulkanInstance, nullptr);
	}

	VkInstance& VulkanInstanceWrapper::GetInstance()
	{
		return vulkanInstance;
	}

	const VkInstance& VulkanInstanceWrapper::GetInstance() const
	{
		return vulkanInstance;
	}
}
