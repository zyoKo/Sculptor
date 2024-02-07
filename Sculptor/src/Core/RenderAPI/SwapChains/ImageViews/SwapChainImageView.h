#pragma once

namespace Sculptor::Core
{
	class LogicalDevice;

	class SwapChain;
}

namespace Sculptor::Core
{
	class SwapChainImageView
	{
	public:
		SwapChainImageView(const std::weak_ptr<LogicalDevice>& device,
			const std::weak_ptr<SwapChain>& swapChain);

		~SwapChainImageView() = default;

		void Create();

		void CleanUp() const;

		const std::vector<VkImageView>& GetSwapChainImageViews() const;

	private:
		std::vector<VkImageView> swapChainImageViews;

		std::weak_ptr<SwapChain> swapChain;

		std::weak_ptr<LogicalDevice> logicalDevice;

		friend class FrameBuffer;
	};
}