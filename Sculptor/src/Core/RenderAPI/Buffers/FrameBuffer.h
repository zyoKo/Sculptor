#pragma once

#include "Utilities/Macros.h"

namespace Sculptor::Core
{
	class SwapChainImageView;
	class LogicalDevice;
	class SwapChain;
	class RenderApi;
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

		void SetSwapChainImageViews(std::weak_ptr<SwapChainImageView> imageViews) noexcept;

		const std::vector<VkFramebuffer>& GetSwapChainFrameBuffers() const;

		void AddImageView(VkImageView newImageView);

		void SetOtherImageViews(const std::vector<VkImageView>& imageViews);

		LOGICAL_DEVICE

		SWAP_CHAIN

		RENDER_API

	private:
		std::vector<VkFramebuffer> swapChainFrameBuffers;

		std::weak_ptr<SwapChainImageView> swapChainImageViews;

		std::vector<VkImageView> otherImageViews;

		FRIEND(CommandBuffer)
	};
}