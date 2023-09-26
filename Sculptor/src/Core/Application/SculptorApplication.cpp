#include <SculptorPch.h>

#include "SculptorApplication.h"

#include "Utilities/ExtensionManager.h"

namespace Sculptor::Core
{
	SculptorApplication::SculptorApplication()
		:	window(std::make_shared<WindowsWindow>()),
			windowProperties(1920, 1080),
			vulkanInstanceWrapper(std::make_shared<VulkanInstanceWrapper>()),
			validationLayer(std::make_shared<ValidationLayer>()),
			windowSurface(std::make_shared<Windows::VulkanWindowSurface>()),
			logicalDevice(std::make_shared<LogicalDevice>(validationLayer, vulkanInstanceWrapper))
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

		validationLayer->SetVulkanInstanceWrapper(vulkanInstanceWrapper);

		validationLayer->SetupDebugMessenger();

		windowSurface->SetNativeWindow(window);

		windowSurface->SetVulkanInstanceWrapper(vulkanInstanceWrapper);

		windowSurface->SetValidationLayer(validationLayer);

		windowSurface->CreateWindowSurface();

		logicalDevice->SetVulkanWindowSurface(windowSurface);

		logicalDevice->CreateLogicalDevice();
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

		windowSurface->CleanUp();

		validationLayer->CleanUp();

		vulkanInstanceWrapper->DestroyInstance();

		window->Shutdown();
	}
}
