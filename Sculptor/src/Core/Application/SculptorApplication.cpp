#include <SculptorPch.h>

#include "SculptorApplication.h"

#include "Utilities/ExtensionManager.h"

namespace Sculptor::Core
{
	SculptorApplication::SculptorApplication()
		:	window(std::make_unique<Window>()),
			windowProperties(1920, 1080),
			vulkanInstanceWrapper(std::make_shared<VulkanInstanceWrapper>()),
			validationLayer(std::make_shared<ValidationLayer>()),
			logicalDevice(std::make_shared<LogicalDevice>(validationLayer, vulkanInstanceWrapper))
	{
		validationLayer->SetVulkanInstanceWrapper(vulkanInstanceWrapper);

		ExtensionManager::Initialize(validationLayer);
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

		validationLayer->SetupDebugMessenger();

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

		validationLayer->CleanUp();

		vulkanInstanceWrapper->DestroyInstance();

		window->Shutdown();
	}
}
