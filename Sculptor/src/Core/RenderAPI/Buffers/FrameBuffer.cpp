#include <SculptorPch.h>

#include <utility>

#include "FrameBuffer.h"

#include "Core/RenderAPI/SwapChains/ImageViews/SwapChainImageView.h"
#include "Core/RenderAPI/RenderApi.h"
#include "Core/RenderAPI/SwapChains/SwapChain.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"

namespace Sculptor::Core
{
	FrameBuffer::FrameBuffer(std::weak_ptr<SwapChainImageView> imageViews, std::weak_ptr<RenderApi> renderApi,
		std::weak_ptr<SwapChain> swapChain, std::weak_ptr<LogicalDevice> logicalDevice) noexcept
		:	swapChain(std::move(swapChain)),
			logicalDevice(std::move(logicalDevice)),
			imageViews(std::move(imageViews)),
			renderApi(std::move(renderApi))
	{ }

	void FrameBuffer::Create()
	{
		GetShared<SwapChainImageView> imageViewPtr{ imageViews };
		const std::vector<VkImageView>& swapChainImageViews = imageViewPtr->swapChainImageViews;

		GetShared<RenderApi> renderApiPtr{ renderApi };
		const auto& renderPass = renderApiPtr->renderPass;

		GetShared<SwapChain> swapChainPtr{ swapChain };
		const auto& swapChainExtent = swapChainPtr->swapChainExtent;

		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
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
			VkFramebufferCreateInfo frameBufferInfo{
				.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
				.renderPass = renderPass,
				.attachmentCount = 1,
				.pAttachments = attachments,
				.width = swapChainExtent.width,
				.height = swapChainExtent.height,
				.layers = 1
			};

			VK_CHECK(vkCreateFramebuffer(device, &frameBufferInfo, nullptr, &swapChainFrameBuffers[i]), "Failed to create framebuffer.")
		}
	}

	void FrameBuffer::CleanUp() const
	{
		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto& device = logicalDevicePtr->Get();

		for (const auto& frameBuffer : swapChainFrameBuffers)
		{
			vkDestroyFramebuffer(device, frameBuffer, nullptr);
		}
	}

	void FrameBuffer::SetImageViews(std::weak_ptr<SwapChainImageView> imageViews) noexcept
	{
		this->imageViews = std::move(imageViews);
	}

	void FrameBuffer::SetRenderApi(std::weak_ptr<RenderApi> renderApi) noexcept
	{
		this->renderApi = std::move(renderApi);
	}

	const std::vector<VkFramebuffer>& FrameBuffer::GetSwapChainFrameBuffers() const
	{
		return swapChainFrameBuffers;
	}
}
