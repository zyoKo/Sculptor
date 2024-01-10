#include <SculptorPch.h>

#include "SculptorApplication.h"

#include "Platform/Windows/WindowData/WindowConstants.h"
#include "Utilities/ExtensionManager.h"
#include "Core/RenderAPI/SwapChains/SwapChain.h"
#include "Core/Windows/VulkanWindowSurface/VulkanWindowSurface.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Core/RenderAPI/Devices/PhysicalDevice.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Core/RenderAPI/ValidationLayer/ValidationLayer.h"
#include "Platform/Windows/WindowData/WindowProperties.h"
#include "Core/RenderAPI/SwapChains/ImageViews/ImageViews.h"
#include "Core/RenderAPI/RenderApi.h"
#include "Core/RenderAPI/Pipelines/Graphics/GraphicsPipeline.h"
#include "Core/RenderAPI/Buffers/FrameBuffer.h"
#include "Core/RenderAPI/Pools/CommandPool.h"

namespace Sculptor::Core
{
	SculptorApplication::SculptorApplication()
		:	window(std::make_shared<WindowsWindow>()),
			vulkanInstanceWrapper(std::make_shared<VulkanInstanceWrapper>()),
			validationLayer(std::make_shared<ValidationLayer>()),
			windowSurface(std::make_shared<Windows::VulkanWindowSurface>()),
			logicalDevice(std::make_shared<LogicalDevice>()),
			swapChains(std::make_shared<SwapChain>()),
			imageViews(std::make_shared<ImageViews>(logicalDevice, swapChains)),
			renderApi(std::make_shared<RenderApi>(swapChains, logicalDevice)),
			graphicsPipeline(std::make_shared<GraphicsPipeline>(renderApi, swapChains, logicalDevice)),
			frameBuffer(std::make_shared<FrameBuffer>(imageViews, renderApi, swapChains, logicalDevice)),
			commandPool(std::make_shared<CommandPool>())
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
		constexpr WindowProperties windowProperties{ WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE };

		window->InitializeWindow(windowProperties);
	}

	void SculptorApplication::InitializeVulkan() const
	{
		vulkanInstanceWrapper->CreateInstance(validationLayer);

		validationLayer->SetupDebugMessenger(vulkanInstanceWrapper);

		windowSurface->CreateWindowSurface(vulkanInstanceWrapper, window);

		// Physical-Device selection abstracted to the method call below
		// Queue-Family Initialization abstracted to method call below
		logicalDevice->CreateLogicalDevice(vulkanInstanceWrapper, validationLayer, windowSurface);

		swapChains->CreateSwapChain(windowSurface, logicalDevice);

		imageViews->CreateImageViews();

		renderApi->CreateRenderPass();

		graphicsPipeline->CreateGraphicsPipeline();

		frameBuffer->CreateFrameBuffer();

		commandPool->CreateCommandPool();
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
		commandPool->CleanUp();

		frameBuffer->CleanUp();

		graphicsPipeline->CleanUp();

		renderApi->CleanUp();

		imageViews->CleanUp();

		swapChains->CleanUp();

		logicalDevice->CleanUp();

		windowSurface->CleanUp(vulkanInstanceWrapper);

		validationLayer->CleanUp(vulkanInstanceWrapper);

		vulkanInstanceWrapper->DestroyInstance();

		window->Shutdown();
	}
}
