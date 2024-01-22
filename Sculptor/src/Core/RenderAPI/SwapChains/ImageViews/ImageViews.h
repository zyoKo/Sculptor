#pragma once

namespace Sculptor::Core
{
	class LogicalDevice;

	class SwapChain;
}

namespace Sculptor::Core
{
	class ImageViews
	{
	public:
		ImageViews(const std::weak_ptr<LogicalDevice>& device,
			const std::weak_ptr<SwapChain>& swapChain);

		~ImageViews() = default;

		void CreateImageViews();

		void CleanUp() const;

		const std::vector<VkImageView>& GetSwapChainImageViews() const;

	private:
		std::vector<VkImageView> swapChainImageViews;

		std::weak_ptr<SwapChain> swapChain;

		std::weak_ptr<LogicalDevice> logicalDevice;

		friend class FrameBuffer;
	};
}