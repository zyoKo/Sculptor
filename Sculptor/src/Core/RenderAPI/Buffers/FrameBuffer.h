#pragma once

namespace Sculptor::Core
{
	class RenderApi;
	class ImageViews;
	class SwapChain;
	class LogicalDevice;
}

namespace Sculptor::Core
{
	class FrameBuffer
	{
	public:
		FrameBuffer() = default;

		FrameBuffer(const std::weak_ptr<ImageViews>& imageViews, const std::weak_ptr<RenderApi>& renderApi,
			const std::weak_ptr<SwapChain>& swapChain, const std::weak_ptr<LogicalDevice>& logicalDevice);

		~FrameBuffer() = default;

		void CreateFrameBuffer();

		void CleanUp() const;

		void SetImageViews(const std::weak_ptr<ImageViews>& imageViews);

		void SetRenderApi(const std::weak_ptr<RenderApi>& renderApi);

		void SetSwapChain(const std::weak_ptr<SwapChain>& swapChain);

		void SetLogicalDevice(const std::weak_ptr<LogicalDevice>& logicalDevice);

	private:
		std::vector<VkFramebuffer> swapChainFrameBuffers;

		std::weak_ptr<ImageViews> imageViews;

		std::weak_ptr<RenderApi> renderApi;

		std::weak_ptr<SwapChain> swapChain;

		std::weak_ptr<LogicalDevice> logicalDevice;
	};
}