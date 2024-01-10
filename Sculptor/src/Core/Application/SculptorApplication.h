#pragma once

namespace Sculptor::Windows
{
	class VulkanWindowSurface;
}

namespace Sculptor::Core
{
	class ImageViews;
	class WindowsWindow;
	class VulkanInstanceWrapper;
	class ValidationLayer;
	class LogicalDevice;
	class SwapChain;
	class RenderApi;
	class GraphicsPipeline;
	class FrameBuffer;
	class CommandPool;
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

		void Sculpt() const;

	private:
		std::shared_ptr<WindowsWindow> window;

		std::shared_ptr<VulkanInstanceWrapper> vulkanInstanceWrapper;

		std::shared_ptr<ValidationLayer> validationLayer;

		std::shared_ptr<Windows::VulkanWindowSurface> windowSurface;

		std::shared_ptr<LogicalDevice> logicalDevice;

		std::shared_ptr<SwapChain> swapChains;

		std::shared_ptr<ImageViews> imageViews;

		std::shared_ptr<RenderApi> renderApi;

		std::shared_ptr<GraphicsPipeline> graphicsPipeline;

		std::shared_ptr<FrameBuffer> frameBuffer;

		std::shared_ptr<CommandPool> commandPool;

		void InitializeWindow() const;

		void InitializeVulkan() const;

		void MainLoop() const;

		void CleanUp() const;
	};
}
