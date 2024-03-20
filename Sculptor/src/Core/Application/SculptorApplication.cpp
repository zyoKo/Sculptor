#include <SculptorPch.h>

#include "SculptorApplication.h"

#include "Core/Locators/CommandPoolLocator.h"
#include "Core/Locators/LogicalDeviceLocator.h"
#include "Core/Locators/SwapChainLocator.h"
#include "Platform/Windows/WindowData/WindowConstants.h"
#include "Utilities/ExtensionManager.h"
#include "Core/RenderAPI/SwapChains/SwapChain.h"
#include "Core/Windows/VulkanWindowSurface/VulkanWindowSurface.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Core/RenderAPI/Devices/PhysicalDevice.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Core/RenderAPI/ValidationLayer/ValidationLayer.h"
#include "Platform/Windows/WindowData/WindowProperties.h"
#include "Core/RenderAPI/SwapChains/ImageViews/SwapChainImageViews.h"
#include "Core/RenderAPI/RenderApi.h"
#include "Core/RenderAPI/Pipelines/Graphics/GraphicsPipeline.h"
#include "Core/RenderAPI/Buffers/FrameBuffer.h"
#include "Core/RenderAPI/Pools/CommandPool.h"
#include "Core/RenderAPI/Buffers/CommandBuffer.h"
#include "Core/RenderAPI/Buffers/UniformBuffer.h"
#include "Core/RenderAPI/Buffers/Structures/UniformBufferObject.h"
#include "Core/RenderAPI/Pools/DescriptorPool.h"
#include "Core/RenderAPI/Image/VulkanTexture.h"
#include "Core/RenderAPI/Utility/CreateInfo.h"
#include "Core/RenderAPI/Features/DepthTesting.h"
#include "Core/RenderAPI/Utility/SupportUtility.h"
#include "Components/Types/Mesh.h"
#include "Core/Data/Constants.h"
#include "Core/RenderAPI/DescriptorSet/DescriptorBuilder.h"
#include "Utilities/Logger/Assert.h"
#include "Core/RenderAPI/DescriptorSet/ResourceBuilder.h"
#include "Core/Tools/EngineTools.h"
#include "Components/Camera/Camera.h"
#include "Core/Input/Input.h"
#include "Core/Utility/Time.h"

namespace Sculptor::Core
{
	SculptorApplication::SculptorApplication()
		:	window(std::make_shared<WindowsWindow>()),
			vulkanInstanceWrapper(std::make_shared<VulkanInstanceWrapper>()),
			validationLayer(std::make_shared<ValidationLayer>()),
			windowSurface(std::make_shared<Windows::VulkanWindowSurface>()),
			logicalDevice(std::make_shared<LogicalDevice>()),
			swapChain(std::make_shared<SwapChain>(logicalDevice)),
			swapChainImageViews(std::make_shared<SwapChainImageViews>(logicalDevice, swapChain)),
			renderApi(std::make_shared<RenderApi>(logicalDevice, swapChain)),
			graphicsPipeline(std::make_shared<GraphicsPipeline>(renderApi, swapChain, logicalDevice)),
			frameBuffer(std::make_shared<FrameBuffer>(swapChainImageViews, renderApi, swapChain, logicalDevice)),
			commandPool(std::make_shared<CommandPool>(logicalDevice)),
			currentFrame(0),
			texture(std::make_shared<VulkanTexture>(logicalDevice, commandPool)),
			depthTest(std::make_shared<DepthTesting>(logicalDevice, swapChain)),
			mesh(std::make_shared<Component::Mesh>(logicalDevice)),
			engineTools(std::make_shared<EngineTools>(logicalDevice)),
			mainCamera(nullptr)
	{
		LogicalDeviceLocator::Provide(logicalDevice);
		CommandPoolLocator::Provide(commandPool);
		SwapChainLocator::Provide(swapChain);

		Component::Camera::GetInstance()->Initialize(window);

		SupportUtility::SetLogicalDevice(logicalDevice);
		depthTest->SetCommandPool(commandPool);
		depthTest->SetLogicalDevice(logicalDevice);

		imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		commandBuffers.reserve(MAX_FRAMES_IN_FLIGHT);
		for (unsigned i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			commandBuffers.emplace_back(std::make_shared<CommandBuffer>(commandPool, logicalDevice, renderApi, frameBuffer, swapChain, graphicsPipeline));

			imageAvailableSemaphores[i].SetLogicalDevice(logicalDevice);
			renderFinishedSemaphores[i].SetLogicalDevice(logicalDevice);
			inFlightFences[i].SetLogicalDevice(logicalDevice);
		}

		graphicsPipeline->SetVertexBuffer(mesh->GetVertexBuffer());
		graphicsPipeline->SetIndexBuffer(mesh->GetIndexBuffer());

		uniformBuffers.reserve(MAX_FRAMES_IN_FLIGHT);
		for (unsigned i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			uniformBuffers.emplace_back(std::make_shared<UniformBuffer>(logicalDevice));
		}

		Utils::ExtensionManager::Initialize(validationLayer);
	}

	void SculptorApplication::Sculpt()
	{
		InitializeWindow();

		InitializeVulkan();

		MainLoop();

		CleanUp();
	}

	void SculptorApplication::InitializeWindow() const
	{
		window->InitializeWindow(std::make_unique<WindowProperties>(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE));
	}

	void SculptorApplication::InitializeVulkan()
	{
		vulkanInstanceWrapper->CreateInstance(validationLayer);

		validationLayer->SetupDebugMessenger(vulkanInstanceWrapper);

		windowSurface->Create(vulkanInstanceWrapper, window);

		logicalDevice->Create(vulkanInstanceWrapper, validationLayer, windowSurface);

		swapChain->Create(windowSurface);

		swapChainImageViews->Create();

		renderApi->Create();

		commandPool->Create();

		frameBuffer->CreateTextureSampler();

		texture->Create();

		// Uniform Buffers
		std::vector<VkBuffer> currentUniformBuffers;
		currentUniformBuffers.reserve(uniformBuffers.size());
		for (const auto& buffer : uniformBuffers)
		{
			constexpr uint64_t uniformBufferSize = sizeof(UniformBufferObject);

			currentUniformBuffers.emplace_back(buffer->Create(uniformBufferSize));
		}

		// Creating Resources for the GPU
		{
			DescriptorBuilder descriptorBuilder{ logicalDevice };

			U32 nextBinding = 0;
			descriptorBuilder.AddUniformBuffer(currentUniformBuffers, nextBinding++, sizeof(UniformBufferObject));
			descriptorBuilder.AddImageSampler(frameBuffer->GetTextureSampler(), nextBinding++, texture->GetTextureImageView());

			resourceBuilder = std::make_shared<ResourceBuilder>(descriptorBuilder.Build("Test"));
		}

		graphicsPipeline->descriptorSetLayoutTest = resourceBuilder->GetDescriptorSetLayout();
		graphicsPipeline->descriptorSetsTest	  = resourceBuilder->GetDescriptorSets();

		graphicsPipeline->Create();

		depthTest->Create();

		frameBuffer->AddImageView(depthTest->GetImageView());

		frameBuffer->Create();

		mesh->CreateBuffers();

		for (unsigned i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			commandBuffers[i]->Create();

			imageAvailableSemaphores[i].Create();
			renderFinishedSemaphores[i].Create();
			inFlightFences[i].Create();
		}

		mainCamera = Component::Camera::GetInstance();
	}

	void SculptorApplication::MainLoop()
	{
		engineTools->Initialize(window->GetGLFWWindow(), vulkanInstanceWrapper->GetInstance(), resourceBuilder->GetDescriptorPool(), renderApi->GetRenderPass());

		while (!window->WindowShouldClose())
		{
			WindowsWindow::PollEvents();

			Input::Update();

			Time::Update();

			mainCamera->Update();

			DrawFrame();
		}

		RecreateSwapChain();
	}

	void SculptorApplication::CleanUp() const
	{
		engineTools->CleanUp();

		CleanUpSwapChain();

		for (unsigned i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			imageAvailableSemaphores[i].Destroy();
			renderFinishedSemaphores[i].Destroy();
			inFlightFences[i].Destroy();
		}

		commandPool->CleanUp();

		graphicsPipeline->CleanUp();

		resourceBuilder->CleanUp();

		renderApi->CleanUp();

		texture->CleanUp();

		frameBuffer->DestroyTextureSampler();

		mesh->CleanUp();

		for (const auto& buffer : uniformBuffers)
		{
			buffer->Destroy();
		}

		logicalDevice->CleanUp();

		windowSurface->CleanUp(vulkanInstanceWrapper);

		validationLayer->CleanUp(vulkanInstanceWrapper);

		vulkanInstanceWrapper->DestroyInstance();

		window->Shutdown();
	}

	void SculptorApplication::DrawFrame()
	{
		graphicsPipeline->SetCurrentFrame(currentFrame);

		inFlightFences[currentFrame].Wait();

		inFlightFences[currentFrame].Reset();

		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(
			logicalDevice->Get(),
			swapChain->Get(),
			UINT32_MAX,
			imageAvailableSemaphores[currentFrame].Get(),
			VK_NULL_HANDLE,
			&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapChain();
			return;
		}
		S_ASSERT(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR, "Failed to acquire swap chain image.")

		uniformBuffers[currentFrame]->Update();

		// Only reset the fence if we are submitting work
		inFlightFences[currentFrame].Reset();

		/*--Can Pull more commands while recording--*/
		commandBuffers[currentFrame]->Record(imageIndex);

		engineTools->RenderWindow(commandBuffers[currentFrame]->GetBuffer());

		commandBuffers[currentFrame]->EndRecord();
		/*------------------------------------------*/

		const VkSemaphore waitSemaphores[]   = { imageAvailableSemaphores[currentFrame].Get() };
		const VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame].Get() };
		constexpr VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		const auto submitInfo = CreateInfo<VkSubmitInfo>({
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = waitSemaphores,
			.pWaitDstStageMask = waitStages,
			.commandBufferCount = 1,
			.pCommandBuffers = &commandBuffers[currentFrame]->GetBuffer(),
			.signalSemaphoreCount = 1,
			.pSignalSemaphores = signalSemaphores
		});

		const auto& graphicsQueue = logicalDevice->GetQueueFamilies().GetGraphicsQueue();
		const auto& presentQueue = logicalDevice->GetQueueFamilies().GetPresentQueue();

		VK_CHECK(vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame].Get()), "Failed to submit draw command buffer.")

		const VkSwapchainKHR swapChains[] = { swapChain->Get() };

		const auto presentInfo = CreateInfo<VkPresentInfoKHR>({
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = signalSemaphores,
			.swapchainCount = 1,
			.pSwapchains = swapChains,
			.pImageIndices = &imageIndex,
			.pResults = VK_NULL_HANDLE // Optional
		});

		result = vkQueuePresentKHR(presentQueue, &presentInfo);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			RecreateSwapChain();
		}

		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void SculptorApplication::CleanUpSwapChain() const
	{
		depthTest->CleanUp();

		frameBuffer->CleanUp();

		swapChainImageViews->Destroy();

		swapChain->CleanUp();
	}

	void SculptorApplication::RecreateSwapChain() const
	{
		int width = 0, height = 0;
		glfwGetFramebufferSize(window->GetGLFWWindow(), &width, &height);
		while (width == 0 || height == 0)
		{
			glfwGetFramebufferSize(window->GetGLFWWindow(), &width, &height);
			glfwWaitEvents();
		}

		VK_CHECK(vkDeviceWaitIdle(logicalDevice->Get()), "Device failed to wait idle.")

		CleanUpSwapChain();

		swapChain->Create(windowSurface);

		swapChainImageViews->Create();

		depthTest->Create();

		frameBuffer->Create();
	}
}
