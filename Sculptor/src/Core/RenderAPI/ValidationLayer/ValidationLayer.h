#pragma once

namespace Sculptor::Core
{
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

	class ValidationLayer
	{
	public:
		ValidationLayer();

		explicit ValidationLayer(const ValidationLayer&) = delete;

		ValidationLayer& operator=(const ValidationLayer&) = delete;

		~ValidationLayer() = default;

		void RequestValidationLayer(VkInstance* vulkanInstance);

		bool IsEnabled() const;

		const std::vector<const char*>& GetValidationLayersArray() const;

		static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		void SetupDebugMessenger();

		VkDebugUtilsMessengerEXT& GetDebugMessenger() { return debugMessenger; }

		void CleanUp() const;

	private:
		std::vector<const char*> validationLayers;

		VkDebugUtilsMessengerEXT debugMessenger;

		VkInstance* vulkanInstance;

		bool enableValidationLayers;

		bool CheckValidationLayerSupport() const;
	};
}
