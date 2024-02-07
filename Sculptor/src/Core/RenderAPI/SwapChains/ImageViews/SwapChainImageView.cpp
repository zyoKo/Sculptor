#include <SculptorPch.h>

#include "SwapChainImageView.h"

#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Core/RenderAPI/SwapChains/SwapChain.h"
#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	SwapChainImageView::SwapChainImageView(const std::weak_ptr<LogicalDevice>& device,
		const std::weak_ptr<SwapChain>& swapChain)
		:	swapChain(swapChain),
			logicalDevice(device)
	{ }

	void SwapChainImageView::Create()
	{
		const auto swapChainPtr = swapChain.lock();
		if (!swapChainPtr)
		{
			std::cerr << "Failed to create Image View cause swap chain is not set." << std::endl;
			return;
		}

		swapChainImageViews.resize(swapChainPtr->swapChainImages.size());

		for (size_t i = 0; i < swapChainPtr->swapChainImages.size(); ++i)
		{
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = swapChainPtr->swapChainImages[i];

			// These two fields specify how the image is interpreted
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = swapChainPtr->swapChainImageFormat;

			// The components field allow for swizzle color channel
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			// Specifies image's purpose and which part of the image should be accessed
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			const auto devicePtr = logicalDevice.lock();
			S_ASSERT(devicePtr == nullptr, "Initialize Logical Device before creating Image Views.");

			const auto& logicalDevicePtr = devicePtr->Get();

			const auto result = vkCreateImageView(logicalDevicePtr, &createInfo, nullptr, &swapChainImageViews[i]);
			S_ASSERT(result != VK_SUCCESS, "Failed to create image views!");
		}
	}

	void SwapChainImageView::CleanUp() const
	{
		const auto logicalDevicePtr = logicalDevice.lock();
		if (!logicalDevicePtr)
		{
			std::cerr << "Can't perform image views cleanup cause logical Device stopped existing." << std::endl;
		}

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
