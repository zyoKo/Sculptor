#include <SculptorPch.h>

#include "CommandBuffer.h"

#include "Core/Core.h"
#include "Core/RenderAPI/Pools/CommandPool.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Utilities/Logger/Assert.h"
#include "Core/RenderAPI/RenderApi.h"
#include "Core/RenderAPI/Buffers/FrameBuffer.h"
#include "Core/RenderAPI/SwapChains/SwapChain.h"
#include "Core/RenderAPI/Pipelines/Graphics/GraphicsPipeline.h"
#include "Data/Constants.h"
#include "Core/Locators/LogicalDeviceLocator.h"
#include "Core/Locators/CommandPoolLocator.h"

namespace Sculptor::Core
{
	CommandBuffer::CommandBuffer(const std::weak_ptr<CommandPool>& commandPool,
		const std::weak_ptr<LogicalDevice>& device, const std::weak_ptr<RenderApi>& renderApi,
		const std::weak_ptr<FrameBuffer>& buffer, const std::weak_ptr<SwapChain>& swapChain,
		const std::weak_ptr<GraphicsPipeline>& pipeline)
		:	commandBuffer(nullptr),
			commandPool(commandPool),
			logicalDevice(device),
			renderApi(renderApi),
			frameBuffer(buffer),
			swapChain(swapChain),
			graphicsPipeline(pipeline)
	{ }

	void CommandBuffer::Create()
	{
		const auto commandPoolPtr = commandPool.lock();
		if (!commandPoolPtr)
		{
			std::cerr << "No reference to command pool provided.\nFailed to create CommandBuffer.\n";
			return;
		}
		const auto pool = commandPoolPtr->commandPool;

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = pool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		const auto logicalDevicePtr = logicalDevice.lock();
		if (!logicalDevicePtr)
		{
			std::cerr << "No reference to logical device.\nFailed to create command buffer.\n";
			return;
		}
		const auto& device = logicalDevicePtr->Get();

		const auto result = vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);
		S_ASSERT(result != VK_SUCCESS, "Failed to create commandBuffer!");
	}

	void CommandBuffer::Record(uint32_t imageIndex) const
	{
		const auto graphicsPipelinePtr = graphicsPipeline.lock();
		if (!graphicsPipelinePtr)
		{
			std::cerr << "Failed to create render pass as reference to graphics pipeline is null.\n";
			return;
		}

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0;	// Optional
		beginInfo.pInheritanceInfo = nullptr;	// Optional (only relevant to secondary command buffer)

		const VkResult result = vkBeginCommandBuffer(commandBuffer, &beginInfo);
		S_ASSERT(result != VK_SUCCESS, "Failed to begin recording command buffer!");

		StartRenderPass(imageIndex);

		graphicsPipelinePtr->BindGraphicsPipeline(*this);

		graphicsPipelinePtr->Draw(*this, static_cast<uint32_t>(VERTICES.size()));

		EndRenderPass();

		End();
	}

	void CommandBuffer::Reset() const
	{
		vkResetCommandBuffer(commandBuffer, 0);
	}

	void CommandBuffer::End() const
	{
		const auto result = vkEndCommandBuffer(commandBuffer);
		S_ASSERT(result != VK_SUCCESS, "Faield to record command buffer!");
	}

	VkCommandBuffer CommandBuffer::BeginSingleTimeCommand(const VkCommandPool& commandPool, const VkDevice& device)
	{
		const VkCommandBufferAllocateInfo allocInfo{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			nullptr,
			commandPool,
			VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			1
		};

		VkCommandBuffer cmdBuffer{};
		VK_CHECK(vkAllocateCommandBuffers(device, &allocInfo, &cmdBuffer), "Failed to allocate single use command buffer")

		constexpr VkCommandBufferBeginInfo beginInfo{
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			nullptr,
			VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
			nullptr
		};

		VK_CHECK(vkBeginCommandBuffer(cmdBuffer, &beginInfo), "Failed to create single use command buffer.")

		return cmdBuffer;
	}

	void CommandBuffer::EndSingleTimeCommand(const VkCommandBuffer& commandBuffer)
	{
		VK_CHECK(vkEndCommandBuffer(commandBuffer), "Failed to end single use command buffer.")

		const VkSubmitInfo submitInfo{
			VK_STRUCTURE_TYPE_SUBMIT_INFO,
			nullptr,
			0,
			nullptr,
			nullptr,
			1,
			&commandBuffer,
			0,
			nullptr
		};

		LOGICAL_DEVICE_LOCATOR

		COMMAND_POOL_LOCATOR

		const auto& graphicsQueue = logicalDevicePtr->GetQueueFamilies().GetGraphicsQueue();

		VK_CHECK(vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE), "Failed to submit info to graphics queue.")
		VK_CHECK(vkQueueWaitIdle(graphicsQueue), "Failed to wait for graphics queue to be idle.")

		vkFreeCommandBuffers(device, cmdPool, 1, &commandBuffer);
	}

	const VkCommandBuffer& CommandBuffer::GetBuffer() const
	{
		return commandBuffer;
	}

	void CommandBuffer::SetCommandPool(const std::weak_ptr<CommandPool>& commandPool)
	{
		this->commandPool = commandPool;
	}

	void CommandBuffer::SetLogicalDevice(const std::weak_ptr<LogicalDevice>& device)
	{
		this->logicalDevice = device;
	}

	void CommandBuffer::SetRenderApi(const std::weak_ptr<RenderApi>& renderApi)
	{
		this->renderApi = renderApi;
	}

	void CommandBuffer::SetFrameBuffer(const std::weak_ptr<FrameBuffer>& buffer)
	{
		this->frameBuffer = buffer;
	}

	void CommandBuffer::SetSwapChain(const std::weak_ptr<SwapChain>& swapChain)
	{
		this->swapChain = swapChain;
	}

	void CommandBuffer::SetGraphicsPipeline(const std::weak_ptr<GraphicsPipeline>& pipeline)
	{
		this->graphicsPipeline = pipeline;
	}

	void CommandBuffer::StartRenderPass(uint32_t imageIndex) const
	{
		const auto renderApiPtr = renderApi.lock();
		if (!renderApiPtr)
		{
			std::cerr << "Failed to start render pass as reference to render Api is null.\n";
			return;
		}
		const auto& renderPass = renderApiPtr->renderPass;

		const auto frameBufferPtr = frameBuffer.lock();
		if (!frameBufferPtr)
		{
			std::cerr << "Failed to start render pass as reference to frame-buffer is null.\n";
			return;
		}
		const auto& swapChainFrameBuffers = frameBufferPtr->swapChainFrameBuffers;

		const auto swapChainPtr = swapChain.lock();
		if (!swapChainPtr)
		{
			std::cerr << "Failed to create render pass as reference to swap chain is null.\n";
			return;
		}
		const auto& swapChainExtent = swapChainPtr->swapChainExtent;

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = swapChainFrameBuffers[imageIndex];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapChainExtent;

		// The first bracket are for initializing the VkColorValue structure
		// Middle bracket are for initializing the VkClearColorValue structure
		// Third bracket to fill in the values
		constexpr VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		// VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS: The render pass commands will be executed from secondary command buffers.
		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void CommandBuffer::EndRenderPass() const
	{
		vkCmdEndRenderPass(commandBuffer);
	}
}
