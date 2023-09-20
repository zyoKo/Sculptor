#include <SculptorPch.h>

#include "ValidationLayer.h"

#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	ValidationLayer::ValidationLayer()
		:	validationLayers{ "VK_LAYER_KHRONOS_validation" },
			debugMessenger(nullptr),
			vulkanInstance(nullptr),
		#ifdef DEBUG
			enableValidationLayers(true)
		#else
			enableValidationLayers(false)
		#endif
	{
	}

	void ValidationLayer::RequestValidationLayer(VkInstance* vulkanInstance)
	{
		S_ASSERT(enableValidationLayers && !CheckValidationLayerSupport(), "Validation Layer Requested, but not available!");

		this->vulkanInstance = vulkanInstance;
	}

	bool ValidationLayer::IsEnabled() const
	{
		return enableValidationLayers;
	}

	bool ValidationLayer::CheckValidationLayerSupport() const
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const auto layerName : validationLayers)
		{
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers)
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
	}

	void ValidationLayer::SetupDebugMessenger()
	{
		if (!enableValidationLayers)
			return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		PopulateDebugMessengerCreateInfo(createInfo);

		const VkResult debugResult = CreateDebugUtilsMessengerEXT(*vulkanInstance, &createInfo, nullptr, &debugMessenger);
		S_ASSERT(debugResult != VK_SUCCESS, "Failed to set up debug messenger!");
	}

	const std::vector<const char*>& ValidationLayer::GetValidationLayersArray() const
	{
		return validationLayers;
	}

	void ValidationLayer::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
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
	}

	// Message CallBack
	VKAPI_ATTR VkBool32 VKAPI_CALL ValidationLayer::DebugCallBack(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			std::cout << "Validation Layer(Verbose): " << pCallbackData->pMessage << std::endl;
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			std::cout << "Validation Layer(Info): " << pCallbackData->pMessage << std::endl;
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			std::cerr << "Validation Layer(Warning): " << pCallbackData->pMessage << std::endl;
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			S_ASSERT(false, pCallbackData->pMessage);
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
			break;
		}

		return VK_FALSE;
	}

	// Proxy Function for Creation of Objects
	VkResult ValidationLayer::CreateDebugUtilsMessengerEXT(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		const auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

		if (func != nullptr)
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);

		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	// Proxy function for Deletions of objects
	void ValidationLayer::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}

	void ValidationLayer::CleanUp()
	{
		if (enableValidationLayers)
			DestroyDebugUtilsMessengerEXT(*vulkanInstance, debugMessenger, nullptr);
	}
}