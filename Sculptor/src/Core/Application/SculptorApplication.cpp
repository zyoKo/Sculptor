#include <SculptorPch.h>

#include "SculptorApplication.h"

#include "Core/Locators/CommandPoolLocator.h"
#include "Core/Locators/DescriptorPoolLocator.h"
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
#include "Core/RenderAPI/SwapChains/ImageViews/ImageViews.h"
#include "Core/RenderAPI/RenderApi.h"
#include "Core/RenderAPI/Pipelines/Graphics/GraphicsPipeline.h"
#include "Core/RenderAPI/Buffers/FrameBuffer.h"
#include "Core/RenderAPI/Pools/CommandPool.h"
#include "Core/RenderAPI/Buffers/CommandBuffer.h"
#include "Core/RenderAPI/Buffers/VertexBuffer.h"
#include "Core/RenderAPI/Buffers/IndexBuffer.h"
#include "Core/RenderAPI/Buffers/UniformBuffer.h"
#include "Core/RenderAPI/Buffers/Data/Constants.h"
#include "Core/RenderAPI/Buffers/Structures/UniformBufferObject.h"
#include "Core/RenderAPI/DescriptorSet/DescriptorSetLayout.h"
#include "Core/RenderAPI/Pools/DescriptorPool.h"
#include "Core/RenderAPI/DescriptorSet/DescriptorSets.h"
#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	constexpr int MAX_FRAMES_IN_FLIGHT = 2;

	SculptorApplication::SculptorApplication()
		:	window(std::make_shared<WindowsWindow>()),
			vulkanInstanceWrapper(std::make_shared<VulkanInstanceWrapper>()),
			validationLayer(std::make_shared<ValidationLayer>()),
			windowSurface(std::make_shared<Windows::VulkanWindowSurface>()),
			logicalDevice(std::make_shared<LogicalDevice>()),
			swapChain(std::make_shared<SwapChain>()),
			imageViews(std::make_shared<ImageViews>(logicalDevice, swapChain)),
			renderApi(std::make_shared<RenderApi>(swapChain, logicalDevice)),
			graphicsPipeline(std::make_shared<GraphicsPipeline>(renderApi, swapChain, logicalDevice)),
			frameBuffer(std::make_shared<FrameBuffer>(imageViews, renderApi, swapChain, logicalDevice)),
			commandPool(std::make_shared<CommandPool>(logicalDevice)),
			currentFrame(0),
			vertexBuffer(std::make_shared<VertexBuffer>(logicalDevice)),
			indexBuffer(std::make_shared<IndexBuffer>()),
			descriptorSetLayout(std::make_shared<DescriptorSetLayout>()),
			descriptorPool(std::make_shared<DescriptorPool>()),
			descriptorSets(std::make_shared<DescriptorSets>())
	{
		LogicalDeviceLocator::Provide(logicalDevice);
		CommandPoolLocator::Provide(commandPool);
		SwapChainLocator::Provide(swapChain);
		DescriptorPoolLocator::Provide(descriptorPool);

		imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		commandBuffers.reserve(MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			commandBuffers.emplace_back(std::make_shared<CommandBuffer>(commandPool, logicalDevice, renderApi, frameBuffer, swapChain, graphicsPipeline));

			imageAvailableSemaphores[i].SetLogicalDevice(logicalDevice);
			renderFinishedSemaphores[i].SetLogicalDevice(logicalDevice);
			inFlightFences[i].SetLogicalDevice(logicalDevice);
		}

		graphicsPipeline->SetVertexBuffer(vertexBuffer);
		graphicsPipeline->SetIndexBuffer(indexBuffer);
		graphicsPipeline->SetDescriptorSetLayout(descriptorSetLayout);
		graphicsPipeline->SetDescriptorSets(descriptorSets);

		uniformBuffers.reserve(MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			uniformBuffers.emplace_back(std::make_shared<UniformBuffer>());
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
		constexpr WindowProperties windowProperties{ WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE };

		window->InitializeWindow(windowProperties);
	}

	void SculptorApplication::InitializeVulkan()
	{
		vulkanInstanceWrapper->CreateInstance(validationLayer);

		validationLayer->SetupDebugMessenger(vulkanInstanceWrapper);

		windowSurface->Create(vulkanInstanceWrapper, window);

		// Physical-Device selection abstracted to the method call below
		// Queue-Family Initialization abstracted to method call below
		logicalDevice->Create(vulkanInstanceWrapper, validationLayer, windowSurface);

		swapChain->Create(windowSurface, logicalDevice);

		imageViews->Create();

		renderApi->Create();

		descriptorSetLayout->Create();

		graphicsPipeline->Create();

		frameBuffer->Create();

		commandPool->Create();

		// Vertex Buffer
		const uint64_t bufferSize = sizeof(VERTICES[0]) * VERTICES.size();
		const BufferProperties vertexBufferProperties{
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};
		vertexBuffer->Create(vertexBufferProperties);

		// Index Buffer
		const uint64_t indexBufferSize = sizeof(INDICES[0]) * INDICES.size();
		const BufferProperties indexBufferProperties{
			indexBufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};
		indexBuffer->Create(indexBufferProperties);

		// Uniform Buffers
		constexpr uint64_t uniformBufferSize = sizeof(UniformBufferObject);
		constexpr BufferProperties uniformBufferProperties{
			uniformBufferSize,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};
		for (const auto& buffer : uniformBuffers)
		{
			buffer->Create(uniformBufferProperties);
		}

		descriptorPool->Create();

		descriptorSets->Allocate(descriptorSetLayout, uniformBuffers, descriptorSets);

		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			commandBuffers[i]->Create();

			imageAvailableSemaphores[i].Create();
			renderFinishedSemaphores[i].Create();
			inFlightFences[i].Create();
		}
	}

	void SculptorApplication::MainLoop()
	{
		while (!window->WindowShouldClose())
		{
			window->PollEvents();

			DrawFrame();
		}

		RecreateSwapChain();
	}

	void SculptorApplication::CleanUp() const
	{
		descriptorSetLayout->CleanUp();

		CleanUpSwapChain();

		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			imageAvailableSemaphores[i].Destroy();
			renderFinishedSemaphores[i].Destroy();
			inFlightFences[i].Destroy();
		}

		commandPool->CleanUp();

		graphicsPipeline->CleanUp();

		descriptorPool->CleanUp();

		descriptorSetLayout->CleanUp();

		renderApi->CleanUp();

		vertexBuffer->CleanUp();

		indexBuffer->CleanUp();

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
		graphicsPipeline->UpdateCurrentFrame(currentFrame);

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

		commandBuffers[currentFrame]->Reset();

		commandBuffers[currentFrame]->Record(imageIndex);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		const VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame].Get() };
		constexpr VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffers[currentFrame]->GetBuffer();

		const VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame].Get() };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		const auto& graphicsQueue = logicalDevice->GetQueueFamilies().GetGraphicsQueue();
		const auto& presentQueue = logicalDevice->GetQueueFamilies().GetPresentQueue();

		result = vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame].Get());
		S_ASSERT(result != VK_SUCCESS, "Failed to submit draw command buffer.");

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		const VkSwapchainKHR swapChains[] = { swapChain->Get() };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr; // Optional

		result = vkQueuePresentKHR(presentQueue, &presentInfo);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			RecreateSwapChain();
		}
		else if (result != VK_SUCCESS)
		{
			S_ASSERT(result != VK_SUCCESS, "Failed to preset swap chain image.");
		}

		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void SculptorApplication::CleanUpSwapChain() const
	{
		frameBuffer->CleanUp();

		imageViews->CleanUp();

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

		vkDeviceWaitIdle(logicalDevice->Get());

		CleanUpSwapChain();

		swapChain->Create(windowSurface, logicalDevice);

		imageViews->Create();

		frameBuffer->Create();
	}
}
