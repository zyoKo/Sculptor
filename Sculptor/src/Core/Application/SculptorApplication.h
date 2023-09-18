#pragma once

#include "Core/RenderAPI/VulkanInstance.h"
#include "Core/Windows/Window.h"
#include "Core/RenderAPI/ValidationLayer/ValidationLayer.h"

namespace Sculptor::Core
{
	class SculptorApplication
	{
	public:
		SculptorApplication();

		~SculptorApplication() = default;

		void Sculpt();

	private:
		std::unique_ptr<Window> window;

		std::unique_ptr<VulkanInstance> vulkanInstance;

		std::unique_ptr<ValidationLayer> validationLayer;

		WindowProperties windowProperties;

		//VkInstance vulkanInstance;

		// CORE FUNCTIONS FOR THIS CLASS///////////////
		void InitializeWindow() const;

		void InitializeVulkan();

		void MainLoop() const;

		void CleanUp() const;
		///////////////////////////////////////////////

		// Move these into appropriate classes ////////
		void CreateVulkanInstance() const;

		//std::vector<const char*> GetRequiredExtensions() const;

		//static void PrintAllSupportedExtensions();

		//static void PrintRequiredGLFWExtensions(const std::vector<const char*>& requiredExtensions);
		//////////////////////////////////////////////
	};
}
