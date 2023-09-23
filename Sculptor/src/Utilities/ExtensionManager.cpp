#include <SculptorPch.h>

#include <GLFW/glfw3.h>

#include "ExtensionManager.h"

#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	void ExtensionManager::Initialize(const std::shared_ptr<ValidationLayer>& validationLayerPtr)
	{
		validationLayer = validationLayerPtr;

		validationLayerIsInitialized = true;
	}

	std::vector<const char*> ExtensionManager::GetRequiredExtensions()
	{
		if (validationLayerIsInitialized)
			S_ASSERT(false, "Please Initialize Validation Layer before requesting extension!");

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> requiredExtensions;
		requiredExtensions.reserve(glfwExtensionCount);
		for (uint32_t i = 0; i < glfwExtensionCount; ++i)
			requiredExtensions.emplace_back(glfwExtensions[i]);

		requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);	// "VK_KHR_portability_enumeration"

		// If Validation Layers are enabled, we need VK_EXT_Debug_util added to the extensions,
		// to setup a callback in the program to handle messages
		if (validationLayer->IsEnabled())
			requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		return requiredExtensions;
	}

	void ExtensionManager::PrintAllSupportedExtensions()
	{
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

		std::cout << "Available Extensions:\n";
		for (const auto& extension : availableExtensions)
		{
			std::cout << extension.extensionName << "\n";
		}
		std::cout << "\n";
	}
}
