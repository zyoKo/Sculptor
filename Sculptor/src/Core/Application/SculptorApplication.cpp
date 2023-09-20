#include <SculptorPch.h>

#include "SculptorApplication.h"

#include "Core/RenderAPI/ExtensionManager.h"

namespace Sculptor::Core
{
	SculptorApplication::SculptorApplication()
		:	window(std::make_unique<Window>()),
			windowProperties(1920, 1080),
			vulkanInstanceWrapper(std::make_shared<VulkanInstanceWrapper>()),
			validationLayer(std::make_shared<ValidationLayer>())
	{
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
		validationLayer->RequestValidationLayer(&vulkanInstanceWrapper->GetInstance());

		vulkanInstanceWrapper->CreateInstance(validationLayer);
		//vulkanInstanceWrapper->CreateInstance(nullptr);

		validationLayer->SetupDebugMessenger();
	}

	void SculptorApplication::MainLoop() const
	{
		while(!window->WindowShouldClose())
		{
			window->PollEvents();
		}
	}

	void SculptorApplication::CleanUp() const
	{
		validationLayer->CleanUp();

		vulkanInstanceWrapper->DestroyInstance();

		window->Shutdown();
	}
}
