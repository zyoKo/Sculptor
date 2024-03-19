#include <SculptorPch.h>

#include <utility>

#include "CommandBuffer.h"

#include "Core/Core.h"
#include "Core/Data/Constants.h"
#include "Core/RenderAPI/Pools/CommandPool.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Core/RenderAPI/RenderApi.h"
#include "Core/RenderAPI/Buffers/FrameBuffer.h"
#include "Core/RenderAPI/SwapChains/SwapChain.h"
#include "Core/RenderAPI/Pipelines/Graphics/GraphicsPipeline.h"
#include "Core/Locators/LogicalDeviceLocator.h"
#include "Core/Locators/CommandPoolLocator.h"
#include "Core/RenderAPI/Utility/CreateInfo.h"

namespace Sculptor::Core
{
	CommandBuffer::CommandBuffer()
		:	commandBuffer(VK_NULL_HANDLE)
	{ }

	CommandBuffer::CommandBuffer(
		std::weak_ptr<CommandPool>		commandPool,
		std::weak_ptr<LogicalDevice>	device,
		std::weak_ptr<RenderApi>		renderApi,
		std::weak_ptr<FrameBuffer>		buffer,
		std::weak_ptr<SwapChain>		swapChain,
		std::weak_ptr<GraphicsPipeline> pipeline) noexcept
		:	logicalDevice	(std::move(device)),
			renderApi		(std::move(renderApi)),
			swapChain		(std::move(swapChain)),
			commandBuffer	(VK_NULL_HANDLE),
			commandPool		(std::move(commandPool)),
			frameBuffer		(std::move(buffer)),
			graphicsPipeline(std::move(pipeline))
	{ }

	void CommandBuffer::Create()
	{
		GetShared<CommandPool> commandPoolPtr{ commandPool };
		const auto pool = commandPoolPtr->commandPool;

		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto& device = logicalDevicePtr->Get();

		const auto commandBufferAllocateInfo = CreateInfo<VkCommandBufferAllocateInfo>({
			.commandPool = pool,
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = 1
		});

		VK_CHECK(vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, &commandBuffer), "Failed to create commandBuffer!")
	}

	void CommandBuffer::Record(uint32_t imageIndex) const
	{
		GetShared<GraphicsPipeline> graphicsPipelinePtr{ graphicsPipeline };

		const auto commandBufferBeginInfo = CreateInfo<VkCommandBufferBeginInfo>({
			.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
		});

		// Reset only if VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT is not set else,
		// we don't need to do it cause it handled from the internal calls from the API
		if ((commandBufferBeginInfo.flags & VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) != VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT)
		{
			Reset();
		}

		VK_CHECK(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo), "Failed to begin recording command buffer!")

		StartRenderPass(imageIndex);

		graphicsPipelinePtr->BindGraphicsPipeline(*this);

		graphicsPipelinePtr->Draw(*this);

		// Call EndRecord Here in the hierarchy
	}

	void CommandBuffer::EndRecord() const
	{
		EndRenderPass();

		End();
	}

	void CommandBuffer::Reset() const
	{
		// Call this if the flag for creating command buffer is not set to VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT

		VK_CHECK(vkResetCommandBuffer(commandBuffer, 0), "Failed to reset command buffer.")
	}

	void CommandBuffer::End() const
	{
		VK_CHECK(vkEndCommandBuffer(commandBuffer), "Faield to record command buffer.")
	}

	VkCommandBuffer CommandBuffer::BeginSingleTimeCommand(VkCommandPool commandPool, VkDevice device)
	{
		VkCommandBuffer cmdBuffer{ VK_NULL_HANDLE };
		const auto commandBufferAllocateInfo = CreateInfo<VkCommandBufferAllocateInfo>({
			.commandPool = commandPool,
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = 1
		});
		VK_CHECK(vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, &cmdBuffer), "Failed to allocate single use command buffer")

		const auto commandBufferBeginInfo = CreateInfo<VkCommandBufferBeginInfo>({
			.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
		});
		VK_CHECK(vkBeginCommandBuffer(cmdBuffer, &commandBufferBeginInfo), "Failed to create single use command buffer.")

		return cmdBuffer;
	}

	void CommandBuffer::EndSingleTimeCommand(VkCommandBuffer commandBuffer)
	{
		VK_CHECK(vkEndCommandBuffer(commandBuffer), "Failed to end single use command buffer.")

		const auto submitInfo = CreateInfo<VkSubmitInfo>({
			.waitSemaphoreCount = 0,
			.pWaitSemaphores = VK_NULL_HANDLE,
			.pWaitDstStageMask = VK_NULL_HANDLE,
			.commandBufferCount = 1,
			.pCommandBuffers = &commandBuffer,
			.signalSemaphoreCount = 0,
			.pSignalSemaphores = VK_NULL_HANDLE
		});

		LOGICAL_DEVICE_LOCATOR
		COMMAND_POOL_LOCATOR

		const auto& graphicsQueue = logicalDevicePtr->GetQueueFamilies().GetGraphicsQueue();
		VK_CHECK(vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE), "Failed to submit info to graphics queue.")
		VK_CHECK(vkQueueWaitIdle(graphicsQueue), "Failed to wait for graphics queue to be idle.")

		vkFreeCommandBuffers(device, cmdPool, 1, &commandBuffer);
	}

	VkCommandBuffer& CommandBuffer::GetBuffer()
	{
		return commandBuffer;
	}

	const VkCommandBuffer& CommandBuffer::GetBuffer() const
	{
		return commandBuffer;
	}

	void CommandBuffer::SetCommandPool(std::weak_ptr<CommandPool> commandPool) noexcept
	{
		this->commandPool = std::move(commandPool);
	}

	void CommandBuffer::SetFrameBuffer(std::weak_ptr<FrameBuffer> buffer) noexcept
	{
		this->frameBuffer = std::move(buffer);
	}

	void CommandBuffer::SetGraphicsPipeline(std::weak_ptr<GraphicsPipeline> pipeline) noexcept
	{
		this->graphicsPipeline = std::move(pipeline);
	}

	void CommandBuffer::StartRenderPass(uint32_t imageIndex) const
	{
		GetShared<RenderApi> renderApiPtr{ renderApi };
		const auto& renderPass = renderApiPtr->renderPass;

		GetShared<FrameBuffer> frameBufferPtr{ frameBuffer };
		const auto& swapChainFrameBuffers = frameBufferPtr->swapChainFrameBuffers;

		GetShared<SwapChain> swapChainPtr{ swapChain };
		const auto& swapChainExtent = swapChainPtr->swapChainExtent;

		// The first bracket are for initializing the VkColorValue structure
		// Middle bracket are for initializing the VkClearColorValue structure
		// Third bracket to fill in the values
		std::array<VkClearValue, 2> clearValue{};
		clearValue[0].color = { DEFAULT_CLEAR_COLOR };
		clearValue[1].depthStencil = { DEFAULT_DEPTH_VALUE, DEFAULT_STENCIL_VALUE };

		const auto renderPassInfo = CreateInfo<VkRenderPassBeginInfo>({
			.renderPass = renderPass,
			.framebuffer = swapChainFrameBuffers[imageIndex],
			.renderArea = {
				.offset = { 0, 0 },
				.extent = swapChainExtent
			},
			.clearValueCount = static_cast<U32>(clearValue.size()),
			.pClearValues = clearValue.data()
		});

		// VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS: The render pass commands will be executed from secondary command buffers.
		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void CommandBuffer::EndRenderPass() const
	{
		vkCmdEndRenderPass(commandBuffer);
	}
}
