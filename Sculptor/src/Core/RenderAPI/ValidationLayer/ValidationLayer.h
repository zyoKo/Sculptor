#pragma once

namespace Sculptor::Core
{
	class ValidationLayer
	{
	public:
		ValidationLayer() = delete;

		ValidationLayer(VkInstance* vkInstance);

		~ValidationLayer() = default;

		void RequestValidationLayer() const;

		bool IsEnabled() const;

		const std::vector<const char*>& GetValidationLayersArray() const;

		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		void CleanUp();

	private:
		std::vector<const char*> validationLayers;

		VkDebugUtilsMessengerEXT debugMessenger;

		VkInstance* vulkanInstance;

		bool enableValidationLayers;

		void SetupDebugMessenger();

		bool CheckValidationLayerSupport() const;

		// Message CallBack
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallBack(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);

		// Proxy Function for Creation of objects
		VkResult CreateDebugUtilsMessengerEXT(
			VkInstance instance,
			const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator,
			VkDebugUtilsMessengerEXT* pDebugMessenger);

		// Proxy function for Deletion of objects
		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

	};
}
