#pragma once

#include "Core/RenderAPI/Async/VulkanFenceWrapper.h"
#include "Core/RenderAPI/Async/VulkanSemaphoreWrapper.h"

namespace Sculptor::Windows
{
	class VulkanWindowSurface;
}

namespace Sculptor::Component
{
	class Mesh;
}

namespace Sculptor::Core
{
	class SwapChainImageViews;
	class WindowsWindow;
	class VulkanInstanceWrapper;
	class ValidationLayer;
	class LogicalDevice;
	class SwapChain;
	class RenderApi;
	class GraphicsPipeline;
	class FrameBuffer;
	class CommandPool;
	class CommandBuffer;
	class UniformBuffer;
	class DescriptorSetLayout;
	class DescriptorPool;
	class DescriptorSets;
	class VulkanTexture;
	class TextureSampler;
	class DepthTesting;
}

namespace Sculptor::Core
{
	class SculptorApplication
	{
	public:
		SculptorApplication();

		~SculptorApplication() = default;

		explicit SculptorApplication(const SculptorApplication&) = delete;

		SculptorApplication& operator=(const SculptorApplication&) = delete;

		SculptorApplication(SculptorApplication&&) = delete;

		SculptorApplication& operator=(SculptorApplication&&) = delete;

		void Sculpt();

	private:
		std::shared_ptr<WindowsWindow> window;

		std::shared_ptr<VulkanInstanceWrapper> vulkanInstanceWrapper;

		std::shared_ptr<ValidationLayer> validationLayer;

		std::shared_ptr<Windows::VulkanWindowSurface> windowSurface;

		std::shared_ptr<LogicalDevice> logicalDevice;

		std::shared_ptr<SwapChain> swapChain;

		std::shared_ptr<SwapChainImageViews> swapChainImageViews;

		std::shared_ptr<RenderApi> renderApi;

		std::shared_ptr<GraphicsPipeline> graphicsPipeline;

		std::shared_ptr<FrameBuffer> frameBuffer;

		std::shared_ptr<CommandPool> commandPool;

		std::vector<std::shared_ptr<CommandBuffer>> commandBuffers;

		// Sync Primitives
		std::vector<VulkanSemaphoreWrapper> imageAvailableSemaphores;
		std::vector<VulkanSemaphoreWrapper> renderFinishedSemaphores;
		std::vector<VulkanFenceWrapper> inFlightFences;

		uint32_t currentFrame;

		std::shared_ptr<VulkanTexture> texture;

		std::vector<std::shared_ptr<UniformBuffer>> uniformBuffers;

		std::shared_ptr<DescriptorSetLayout> descriptorSetLayout;

		std::shared_ptr<DescriptorPool> descriptorPool;

		std::shared_ptr<DescriptorSets> descriptorSets;

		std::shared_ptr<DepthTesting> depthTest;

		std::shared_ptr<Component::Mesh> mesh;

		void InitializeWindow() const;

		void InitializeVulkan();

		void MainLoop();

		void CleanUp() const;

		// TODO: Find a better place for this after tutorial
		void DrawFrame();

		// TODO: Find a better place for this
		void CleanUpSwapChain() const;

		void RecreateSwapChain() const;
	};
}
