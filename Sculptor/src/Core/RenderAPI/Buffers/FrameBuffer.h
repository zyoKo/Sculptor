#pragma once

#include "Utilities/Macros.h"

namespace Sculptor::Core
{
	class SwapChainImageViews;
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

		FrameBuffer(std::weak_ptr<SwapChainImageViews> imageViews,
					std::weak_ptr<RenderApi> renderApi,
					std::weak_ptr<SwapChain> swapChain,
					std::weak_ptr<LogicalDevice> logicalDevice) noexcept;

		~FrameBuffer();

		void Create();

		void CreateTextureSampler();

		void DestroyTextureSampler();

		void CleanUp() const;

		void SetSwapChainImageViews(std::weak_ptr<SwapChainImageViews> imageViews) noexcept;

		const std::vector<VkFramebuffer>& GetSwapChainFrameBuffers() const;

		void AddImageView(VkImageView newImageView);

		void SetOtherImageViews(const std::vector<VkImageView>& imageViews);

		VkSampler GetTextureSampler() const;

		LOGICAL_DEVICE

		SWAP_CHAIN

		RENDER_API

	private:
		std::vector<VkFramebuffer> swapChainFrameBuffers;

		std::weak_ptr<SwapChainImageViews> swapChainImageViews;

		std::vector<VkImageView> otherImageViews;

		VkSampler textureSampler;

		FRIEND(CommandBuffer)
	};
}