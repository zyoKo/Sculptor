#pragma once

#include "Core/RenderAPI/Async/VulkanFenceWrapper.h"
#include "Core/RenderAPI/Async/VulkanSemaphoreWrapper.h"

namespace Sculptor::Windows
{
	class VulkanWindowSurface;
}

namespace Sculptor::Core
{
	class VertexBuffer;
	class SwapChainImageView;
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
	class IndexBuffer;
	class UniformBuffer;
	class DescriptorSetLayout;
	class DescriptorPool;
	class DescriptorSets;
	class VulkanTexture;
	class TextureImageView;
	class TextureSampler;
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

		std::shared_ptr<SwapChainImageView> swapChainImageViews;

		std::shared_ptr<RenderApi> renderApi;

		std::shared_ptr<GraphicsPipeline> graphicsPipeline;

		std::shared_ptr<FrameBuffer> frameBuffer;

		std::shared_ptr<CommandPool> commandPool;

		std::vector<std::shared_ptr<CommandBuffer>> commandBuffers;

		std::vector<VulkanSemaphoreWrapper> imageAvailableSemaphores;
		std::vector<VulkanSemaphoreWrapper> renderFinishedSemaphores;
		std::vector<VulkanFenceWrapper> inFlightFences;

		uint32_t currentFrame;

		std::shared_ptr<VulkanTexture> texture;

		std::shared_ptr<TextureImageView> textureImageView;

		std::shared_ptr<TextureSampler> textureSampler;

		std::shared_ptr<VertexBuffer> vertexBuffer;

		std::shared_ptr<IndexBuffer> indexBuffer;

		std::vector<std::shared_ptr<UniformBuffer>> uniformBuffers;

		std::shared_ptr<DescriptorSetLayout> descriptorSetLayout;

		std::shared_ptr<DescriptorPool> descriptorPool;

		std::shared_ptr<DescriptorSets> descriptorSets;

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
