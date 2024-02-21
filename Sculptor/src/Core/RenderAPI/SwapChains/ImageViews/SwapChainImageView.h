#pragma once

#include "Core/RenderAPI/Image/ImageView.h"
#include "Utilities/Macros.h"

namespace Sculptor::Core
{
	class LogicalDevice;
	class SwapChain;
}

namespace Sculptor::Core
{
	class SwapChainImageView : public ImageView
	{
	public:
		SwapChainImageView(std::weak_ptr<LogicalDevice> device, std::weak_ptr<SwapChain> swapChain) noexcept;

		void Create();

		void Destroy() const override;

		const std::vector<VkImageView>& GetSwapChainImageViews() const;

		LOGICAL_DEVICE

		SWAP_CHAIN

	private:
		std::vector<VkImageView> swapChainImageViews;
		
		FRIEND(FrameBuffer)
	};
}