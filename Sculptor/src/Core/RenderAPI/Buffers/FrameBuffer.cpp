#include <SculptorPch.h>

#include "FrameBuffer.h"

#include "Core/RenderAPI/SwapChains/ImageViews/ImageViews.h"
#include "Core/RenderAPI/RenderApi.h"
#include "Core/RenderAPI/SwapChains/SwapChain.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	FrameBuffer::FrameBuffer(const std::weak_ptr<ImageViews>& imageViews, const std::weak_ptr<RenderApi>& renderApi,
		const std::weak_ptr<SwapChain>& swapChain, const std::weak_ptr<LogicalDevice>& logicalDevice)
		:	imageViews(imageViews),
			renderApi(renderApi),
			swapChain(swapChain),
			logicalDevice(logicalDevice)
	{ }

	void FrameBuffer::Create()
	{
		const auto imageViewPtr = imageViews.lock();
		if (!imageViewPtr)
		{
			std::cerr << "Swap-chain reference is null." << std::endl;
			return;
		}
		const std::vector<VkImageView>& swapChainImageViews = imageViewPtr->swapChainImageViews;

		const auto renderApiPtr = renderApi.lock();
		if (!renderApiPtr)
		{
			std::cerr << "RenderApi reference is null." << std::endl;
			return;
		}
		const auto& renderPass = renderApiPtr->renderPass;

		const auto swapChainPtr = swapChain.lock();
		if (!swapChainPtr)
		{
			std::cerr << "RenderApi reference is null." << std::endl;
			return;
		}
		const auto& swapChainExtent = swapChainPtr->swapChainExtent;

		const auto logicalDevicePtr = logicalDevice.lock();
		if (!logicalDevicePtr)
		{
			std::cerr << "Logical device reference is null." << std::endl;
			return;
		}
		const auto& device = logicalDevicePtr->Get();

		swapChainFrameBuffers.resize(swapChainImageViews.size());

		for (size_t i = 0; i < swapChainImageViews.size(); ++i)
		{
			const VkImageView attachments[] = {
				swapChainImageViews[i]
			};

			// Need to make sure the renderPass is compatible with frameBuffer
			// that is, they use same number and type of attachments
			// TODO: Find a way to make sure renderPass attachment number is same as frameBuffer attachment number here
			VkFramebufferCreateInfo frameBufferInfo{};
			frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			frameBufferInfo.renderPass = renderPass;
			frameBufferInfo.attachmentCount = 1;
			frameBufferInfo.pAttachments = attachments;
			frameBufferInfo.width = swapChainExtent.width;
			frameBufferInfo.height = swapChainExtent.height;
			frameBufferInfo.layers = 1;

			const auto result = vkCreateFramebuffer(device, &frameBufferInfo, nullptr, &swapChainFrameBuffers[i]);
			S_ASSERT(result != VK_SUCCESS, "Failed to create framebuffer.");
		}
	}

	void FrameBuffer::CleanUp() const
	{
		const auto logicalDevicePtr = logicalDevice.lock();
		if (!logicalDevicePtr)
		{
			std::cerr << "Logical device reference is null." << std::endl;
			return;
		}
		const auto& device = logicalDevicePtr->Get();

		for (const auto& frameBuffer : swapChainFrameBuffers)
		{
			vkDestroyFramebuffer(device, frameBuffer, nullptr);
		}
	}

	void FrameBuffer::SetImageViews(const std::weak_ptr<ImageViews>& imageViews)
	{
		this->imageViews = imageViews;
	}

	void FrameBuffer::SetRenderApi(const std::weak_ptr<RenderApi>& renderApi)
	{
		this->renderApi = renderApi;
	}

	void FrameBuffer::SetSwapChain(const std::weak_ptr<SwapChain>& swapChain)
	{
		this->swapChain = swapChain;
	}

	void FrameBuffer::SetLogicalDevice(const std::weak_ptr<LogicalDevice>& logicalDevice)
	{
		this->logicalDevice = logicalDevice;
	}

	const std::vector<VkFramebuffer>& FrameBuffer::GetSwapChainFrameBuffers() const
	{
		return swapChainFrameBuffers;
	}
}
