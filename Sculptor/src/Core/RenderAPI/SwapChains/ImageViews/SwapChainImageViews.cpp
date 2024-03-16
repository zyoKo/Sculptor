#include <SculptorPch.h>

#include "SwapChainImageViews.h"

#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Core/RenderAPI/SwapChains/SwapChain.h"

namespace Sculptor::Core
{
	SwapChainImageViews::SwapChainImageViews(std::weak_ptr<LogicalDevice> device, std::weak_ptr<SwapChain> swapChain) noexcept
		:	logicalDevice(std::move(device)),
			swapChain(std::move(swapChain))
	{ }

	void SwapChainImageViews::Create()
	{
		GetShared<SwapChain> swapChainPtr{ swapChain };
		const auto& swapChainImages = swapChainPtr->swapChainImages;
		const auto swapChainImageFormat = swapChainPtr->swapChainImageFormat;

		swapChainImageViews.resize(swapChainImages.size());

		for (size_t i = 0; i < swapChainImages.size(); ++i)
		{
			swapChainImageViews[i] = CreateImageView(logicalDevice, swapChainImages[i], swapChainImageFormat);
		}
	}

	void SwapChainImageViews::Destroy() const
	{
		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto& device = logicalDevicePtr->Get();

		for (auto& imageView : swapChainImageViews)
		{
			vkDestroyImageView(device, imageView, nullptr);
		}
	}

	const std::vector<VkImageView>& SwapChainImageViews::GetSwapChainImageViews() const
	{
		return swapChainImageViews;
	}
}
