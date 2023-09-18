#pragma once

#include "Core/Windows/Window.h"
#include "Core/RenderAPI/ValidationLayer/ValidationLayer.h"

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

		std::unique_ptr<ValidationLayer> validationLayer;

		WindowProperties windowProperties;

		VkInstance vulkanInstance;

		//std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

		//VkDebugUtilsMessengerEXT debugMessenger;

//#ifdef DEBUG
//		bool enableValidationLayers = true;
//#else
//		bool enableValidationLayers = false;
//#endif

		// CORE FUNCTIONS FOR THIS CLASS///////////////
		void InitializeWindow() const;

		void InitializeVulkan();

		void MainLoop() const;

		void CleanUp() const;
		///////////////////////////////////////////////

		// Move these into appropriate classes ////////
		void CreateVulkanInstance();

		//bool CheckValidationLayerSupport() const;

		std::vector<const char*> GetRequiredExtensions() const;

		static void PrintAllSupportedExtensions();

		static void PrintRequiredGLFWExtensions(const std::vector<const char*>& requiredExtensions);

		//void SetupDebugMessenger();

		//void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		//////////////////////////////////////////////
	};
}
