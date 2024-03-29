#include <SculptorPch.h>

#include "ValidationLayer.h"

#include "Core/Data/Constants.h"
#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	ValidationLayer::ValidationLayer()
		:	validationLayers{ VALIDATION_LAYERS },
			debugMessenger(nullptr),
			enableValidationLayers(false)
	{
#ifdef DEBUG
		enableValidationLayers = true; /* true */
#endif
	}

	bool ValidationLayer::RequestValidationLayer() const
	{
		if (enableValidationLayers && !CheckValidationLayerSupport())
		{
			// TODO: Replace with Logger
			std::cerr << "Validation Layer Requested, but not available!\n";

			return false;
		}

		return true;
	}

	bool ValidationLayer::IsEnabled() const
	{
		return enableValidationLayers;
	}

	void ValidationLayer::SetupDebugMessenger(const std::weak_ptr<VulkanInstanceWrapper>& vulkanInstanceWrapper)
	{
		GetShared<VulkanInstanceWrapper> vulkanInstance{ vulkanInstanceWrapper };

		if (!enableValidationLayers)
			return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		PopulateDebugMessengerCreateInfo(createInfo);

		VK_CHECK(CreateDebugUtilsMessengerEXT(vulkanInstance->GetInstance(), &createInfo, VK_NULL_HANDLE, &debugMessenger),
			"Failed to set up debug messenger!")
	}

	void ValidationLayer::CleanUp(const std::shared_ptr<VulkanInstanceWrapper>& vulkanInstanceWrapper) const
	{
 		if (enableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(vulkanInstanceWrapper->GetInstance(), debugMessenger, nullptr);
		}
	}

	// Private Functions && Message Callback Functions
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

		createInfo.pNext = nullptr;
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

	// Message CallBack
	VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallBack(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			std::cerr << "Validation Layer(Verbose): " << pCallbackData->pMessage << std::endl;
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			std::cerr << "Validation Layer(Info): " << pCallbackData->pMessage << std::endl;
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			std::cerr << "Validation Layer(Warning): " << pCallbackData->pMessage << std::endl;
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			std::cerr << "Validation Layer(Error): " << pCallbackData->pMessage << std::endl;
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
			break;
		}

		return VK_FALSE;
	}

	// Proxy Function for Creation of Objects
	VkResult CreateDebugUtilsMessengerEXT(
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
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}
}