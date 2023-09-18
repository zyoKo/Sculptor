#include <SculptorPch.h>

#include "SculptorApplication.h"

#include "Utilities/Logger/Assert.h"

// TODO: I wanna "PUKE" --> NEED to FIX this tomorrow
namespace Sculptor::Core
{
	//// Message CallBack
	//VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallBack(
	//	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	//	VkDebugUtilsMessageTypeFlagsEXT messageType,
	//	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	//	void* pUserData)
	//{
	//	switch(messageSeverity)
	//	{
	//	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
	//		//std::cout << "Validation Layer(Verbose): " << pCallbackData->pMessage << std::endl;
	//		break;

	//	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
	//		//std::cout << "Validation Layer(Info): " << pCallbackData->pMessage << std::endl;
	//		break;

	//	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
	//		std::cerr << "Validation Layer(Warning): " << pCallbackData->pMessage << std::endl;
	//		break;

	//	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
	//		S_ASSERT(false, pCallbackData->pMessage);
	//		break;

	//	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
	//		break;
	//	}

	//	return VK_FALSE;
	//}

	//// Proxy Function for Creation of Objects
	//VkResult CreateDebugUtilsMessengerEXT(
	//	VkInstance instance, 
	//	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
	//	const VkAllocationCallbacks* pAllocator, 
	//	VkDebugUtilsMessengerEXT* pDebugMessenger)
	//{
	//	const auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	//	if (func != nullptr) 
	//		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	//	
	//	return VK_ERROR_EXTENSION_NOT_PRESENT;
	//}

	//// Proxy function for Deletions of objects
	//void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	//	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	//	if (func != nullptr) {
	//		func(instance, debugMessenger, pAllocator);
	//	}
	//}

	////// MAIN APPLICATION ////////////////////////////////////////////////////////////////////////////////////////////////////////
	SculptorApplication::SculptorApplication()
		:	window(nullptr),
			windowProperties(1920, 1080),
			vulkanInstance(nullptr)
	{
		window = std::make_unique<Window>();

		validationLayer = std::make_unique<ValidationLayer>(&vulkanInstance);
	}

	void SculptorApplication::SculptApplication()
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
		PrintAllSupportedExtensions();

		CreateVulkanInstance();
	}

	void SculptorApplication::MainLoop() const
	{
		while(!window->WindowShouldClose())
		{
			window->PollEvents();
		}
	}

	void SculptorApplication::CleanUp() const
	{
		/*if (enableValidationLayers)
			DestroyDebugUtilsMessengerEXT(vulkanInstance, debugMessenger, nullptr);*/
		validationLayer->CleanUp();

		vkDestroyInstance(vulkanInstance, nullptr);

		window->Shutdown();
	}

	void SculptorApplication::CreateVulkanInstance()
	{
		// TODO: Create a function in Validation Layer to check for ValidationLayer Existence
		//S_ASSERT(enableValidationLayers && !CheckValidationLayerSupport(), "Validation Layer requested, but not available");

		// Create Application Info
		VkApplicationInfo applicationInfo{};
		applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		applicationInfo.pApplicationName = "Sculptor Triangle";
		applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.pEngineName = "Sculptor-Engine";
		applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.apiVersion = VK_API_VERSION_1_3;

		// Create Info needs both applicationInfo and glfwExtensions
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &applicationInfo;

		// Get GLFW Extension Count and Required Extensions
		const auto requiredExtensions = GetRequiredExtensions();
		
		createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
		createInfo.ppEnabledExtensionNames = requiredExtensions.data();

		createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		// Get EnableValidationLayer bool value and the features here
		// Uncomment these to find the issue
		if (validationLayer->IsEnabled())
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayer->GetValidationLayersArray().size());
			createInfo.ppEnabledLayerNames = validationLayer->GetValidationLayersArray().data();

			//PopulateDebugMessengerCreateInfo(debugCreateInfo);
			validationLayer->PopulateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = &debugCreateInfo;
		}
		else
		{
			createInfo.enabledLayerCount = 0;

			createInfo.pNext = nullptr;
		}
		
		// Finally create VulkanInstance
		const VkResult result = vkCreateInstance(&createInfo, nullptr, &vulkanInstance);
		S_ASSERT(result != VK_SUCCESS, "Failed to Create Vulkan Instance!");
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*bool SculptorApplication::CheckValidationLayerSupport() const
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for(auto layerName : validationLayers)
		{
			bool layerFound = false;

			for(const auto& layerProperties : availableLayers)
			{
				if (std::strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
				return false;
		}

		return true;
	}*/

	std::vector<const char*> SculptorApplication::GetRequiredExtensions() const
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> requiredExtensions;
		requiredExtensions.reserve(glfwExtensionCount);
		for (uint32_t i = 0; i < glfwExtensionCount; ++i)
		{
			requiredExtensions.emplace_back(glfwExtensions[i]);
		}

		requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);	// "VK_KHR_portability_enumeration"

		// If Validation Layers are enabled, we need VK_EXT_Debug_util added to the extensions,
		// to setup a callback in the program to handle messages
		// TODO: Get EnableValidationLayer boolean and fix this
		/*if (enableValidationLayers)
		{
			requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}*/

		if (validationLayer->IsEnabled())
		{
			requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		// All required extensions added, now print it
		PrintRequiredGLFWExtensions(requiredExtensions);

		return requiredExtensions;
	}

	void SculptorApplication::PrintAllSupportedExtensions()
	{
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

		std::cout << "Available Extensions:\n";
		for(const auto& extension : availableExtensions)
		{
			std::cout << extension.extensionName << "\n";
		}
		std::cout << "\n";
	}

	void SculptorApplication::PrintRequiredGLFWExtensions(const std::vector<const char*>& requiredExtensions)
	{
		std::cout << "Required GLFW Extension List:\n";
		for (const auto& requiredExtension : requiredExtensions)
		{
			std::cout << requiredExtension << "\n";
		}
		std::cout << "\n";
	}

	/*void SculptorApplication::SetupDebugMessenger()
	{
		if (!enableValidationLayers)
			return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		PopulateDebugMessengerCreateInfo(createInfo);

		const VkResult debugResult = CreateDebugUtilsMessengerEXT(vulkanInstance, &createInfo, nullptr, &debugMessenger);
		S_ASSERT(debugResult != VK_SUCCESS, "failed to set up debug messenger!");
	}

	void SculptorApplication::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

		createInfo.messageSeverity =
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

		createInfo.messageType =
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

		createInfo.pfnUserCallback = DebugCallBack;

		createInfo.pUserData = nullptr;
	}*/
}
