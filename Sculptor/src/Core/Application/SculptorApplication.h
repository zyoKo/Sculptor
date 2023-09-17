#pragma once

#include "Core/Windows/Window.h"

namespace Sculptor::Core
{
	class SculptorApplication
	{
	public:
		SculptorApplication();

		~SculptorApplication() = default;

		void SculptApplication();

	private:
		std::unique_ptr<Window> window;

		WindowProperties windowProperties;

		VkInstance vulkanInstance;

		std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

		VkDebugUtilsMessengerEXT debugMessenger;

#ifdef DEBUG
		bool enableValidationLayers = true;
#else
		bool enableValidationLayers = false;
#endif

		void InitializeWindow() const;

		void InitializeVulkan();

		void MainLoop() const;

		void CleanUp() const;

		// Move it into another file
		void CreateVulkanInstance();

		bool CheckValidationLayerSupport() const;

		std::vector<const char*> GetRequiredExtensions() const;

		static void PrintAllSupportedExtensions();

		static void PrintRequiredGLFWExtensions(const std::vector<const char*>& requiredExtensions);

		void SetupDebugMessenger();

		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	};
}
