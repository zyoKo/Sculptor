#include <SculptorPch.h>

#include <utility>

#include "FrameBuffer.h"

#include "Core/RenderAPI/SwapChains/ImageViews/SwapChainImageView.h"
#include "Core/RenderAPI/RenderApi.h"
#include "Core/RenderAPI/SwapChains/SwapChain.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Core/RenderAPI/Utility/CreateInfo.h"

namespace Sculptor::Core
{
	FrameBuffer::FrameBuffer(std::weak_ptr<SwapChainImageView> imageViews, std::weak_ptr<RenderApi> renderApi,
		std::weak_ptr<SwapChain> swapChain, std::weak_ptr<LogicalDevice> logicalDevice) noexcept
		:	logicalDevice(std::move(logicalDevice)),
			swapChain(std::move(swapChain)),
			renderApi(std::move(renderApi)),
			swapChainImageViews(std::move(imageViews))
	{ }

	void FrameBuffer::Create()
	{
		GetShared<SwapChainImageView> imageViewPtr{ swapChainImageViews };
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
			std::vector<VkImageView> attachments = { swapChainImageViews[i] };

			for (const auto& otherImageView : otherImageViews)
			{
				attachments.emplace_back(otherImageView);
			}

			// Need to make sure the renderPass is compatible with frameBuffer
			// that is, they use same number and type of attachments
			// TODO: Find a way to make sure renderPass attachment number is same as frameBuffer attachment number here
			const auto frameBufferInfo = CreateInfo<VkFramebufferCreateInfo>({
				.renderPass = renderPass,
				.attachmentCount = static_cast<U32>(attachments.size()),
				.pAttachments = attachments.data(),
				.width = swapChainExtent.width,
				.height = swapChainExtent.height,
				.layers = 1
			});

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

	void FrameBuffer::SetSwapChainImageViews(std::weak_ptr<SwapChainImageView> imageViews) noexcept
	{
		this->swapChainImageViews = std::move(imageViews);
	}

	const std::vector<VkFramebuffer>& FrameBuffer::GetSwapChainFrameBuffers() const
	{
		return swapChainFrameBuffers;
	}

	void FrameBuffer::AddImageView(VkImageView newImageView)
	{
		otherImageViews.emplace_back(newImageView);
	}

	void FrameBuffer::SetOtherImageViews(const std::vector<VkImageView>& imageViews)
	{
		otherImageViews = imageViews;
	}
}
