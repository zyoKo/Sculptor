#include <SculptorPch.h>

#include "SculptorApplication.h"

#include "Platform/Windows/WindowData/WindowConstants.h"
#include "Utilities/ExtensionManager.h"

namespace Sculptor::Core
{
	SculptorApplication::SculptorApplication()
		:	window(std::make_shared<WindowsWindow>()),
			windowProperties{ WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE },
			vulkanInstanceWrapper(std::make_shared<VulkanInstanceWrapper>()),
			validationLayer(std::make_shared<ValidationLayer>()),
			windowSurface(std::make_shared<Windows::VulkanWindowSurface>()),
			logicalDevice(std::make_shared<LogicalDevice>())
	{
		Utils::ExtensionManager::Initialize(validationLayer);
	}

	void SculptorApplication::Sculpt() const
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

	void SculptorApplication::InitializeVulkan() const
	{
		vulkanInstanceWrapper->CreateInstance(validationLayer);

		validationLayer->SetupDebugMessenger(vulkanInstanceWrapper);

		windowSurface->CreateWindowSurface(vulkanInstanceWrapper, window);

		logicalDevice->CreateLogicalDevice(vulkanInstanceWrapper, validationLayer, windowSurface);
	}

	void SculptorApplication::MainLoop() const
	{
		while (!window->WindowShouldClose())
		{
			window->PollEvents();
		}
	}

	void SculptorApplication::CleanUp() const
	{
		logicalDevice->CleanUp();

		windowSurface->CleanUp(vulkanInstanceWrapper);

		validationLayer->CleanUp(vulkanInstanceWrapper);

		vulkanInstanceWrapper->DestroyInstance();

		window->Shutdown();
	}
}
