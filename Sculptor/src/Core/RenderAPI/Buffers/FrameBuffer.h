#pragma once

#include "Utilities/Macros.h"

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

		FrameBuffer(std::weak_ptr<SwapChainImageView> imageViews,
					std::weak_ptr<RenderApi> renderApi,
					std::weak_ptr<SwapChain> swapChain,
					std::weak_ptr<LogicalDevice> logicalDevice) noexcept;

		~FrameBuffer() = default;

		void Create();

		void CleanUp() const;

		void SetImageViews(std::weak_ptr<SwapChainImageView> imageViews) noexcept;

		void SetRenderApi(std::weak_ptr<RenderApi> renderApi) noexcept;

		const std::vector<VkFramebuffer>& GetSwapChainFrameBuffers() const;

		SWAP_CHAIN

		LOGICAL_DEVICE

	private:
		std::vector<VkFramebuffer> swapChainFrameBuffers;

		std::weak_ptr<SwapChainImageView> imageViews;

		std::weak_ptr<RenderApi> renderApi;

		FRIEND(CommandBuffer)
	};
}