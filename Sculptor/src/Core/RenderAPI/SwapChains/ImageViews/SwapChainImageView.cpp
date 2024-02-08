#include <SculptorPch.h>

#include "SwapChainImageView.h"

#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Core/RenderAPI/SwapChains/SwapChain.h"

namespace Sculptor::Core
{
	SwapChainImageView::SwapChainImageView(std::weak_ptr<LogicalDevice> device, std::weak_ptr<SwapChain> swapChain) noexcept
		:	logicalDevice(std::move(device)),
			swapChain(std::move(swapChain))
	{ }

	void SwapChainImageView::Create()
	{
		GetShared<SwapChain> swapChainPtr{ swapChain };

		swapChainImageViews.resize(swapChainPtr->swapChainImages.size());

		for (size_t i = 0; i < swapChainPtr->swapChainImages.size(); ++i)
		{
			swapChainImageViews[i] = CreateImageView(swapChainPtr->swapChainImages[i], swapChainPtr->swapChainImageFormat);
		}
	}

	void SwapChainImageView::Destroy() const
	{
		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };

		for (auto& imageView : swapChainImageViews)
		{
			vkDestroyImageView(logicalDevicePtr->Get(), imageView, nullptr);
		}
	}

	const std::vector<VkImageView>& SwapChainImageView::GetSwapChainImageViews() const
	{
		return swapChainImageViews;
	}
}
