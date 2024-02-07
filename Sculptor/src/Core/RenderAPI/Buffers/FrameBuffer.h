#pragma once

namespace Sculptor::Core
{
	class RenderApi;
	class SwapChainImageView;
	class SwapChain;
	class LogicalDevice;
}

namespace Sculptor::Core
{
	class FrameBuffer
	{
	public:
		FrameBuffer() = default;

		FrameBuffer(const std::weak_ptr<SwapChainImageView>& imageViews, const std::weak_ptr<RenderApi>& renderApi,
			const std::weak_ptr<SwapChain>& swapChain, const std::weak_ptr<LogicalDevice>& logicalDevice);

		~FrameBuffer() = default;

		void Create();

		void CleanUp() const;

		void SetImageViews(const std::weak_ptr<SwapChainImageView>& imageViews);

		void SetRenderApi(const std::weak_ptr<RenderApi>& renderApi);

		void SetSwapChain(const std::weak_ptr<SwapChain>& swapChain);

		void SetLogicalDevice(const std::weak_ptr<LogicalDevice>& logicalDevice);

		const std::vector<VkFramebuffer>& GetSwapChainFrameBuffers() const;

	private:
		std::vector<VkFramebuffer> swapChainFrameBuffers;

		std::weak_ptr<SwapChainImageView> imageViews;

		std::weak_ptr<RenderApi> renderApi;

		std::weak_ptr<SwapChain> swapChain;

		std::weak_ptr<LogicalDevice> logicalDevice;

		friend class CommandBuffer;
	};
}