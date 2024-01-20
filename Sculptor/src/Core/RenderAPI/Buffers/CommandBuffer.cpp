#include <SculptorPch.h>

#include "CommandBuffer.h"

#include "Core/RenderAPI/Pools/CommandPool.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Utilities/Logger/Assert.h"
#include "Core/RenderAPI/RenderApi.h"
#include "Core/RenderAPI/Buffers/FrameBuffer.h"
#include "Core/RenderAPI/SwapChains/SwapChain.h"
#include "Core/RenderAPI/Pipelines/Graphics/GraphicsPipeline.h"

namespace Sculptor::Core
{
	CommandBuffer::CommandBuffer(const std::weak_ptr<CommandPool>& commandPool,
		const std::weak_ptr<LogicalDevice>& device, const std::weak_ptr<RenderApi>& renderApi,
		const std::weak_ptr<FrameBuffer>& buffer, const std::weak_ptr<SwapChain>& swapChain,
		const std::weak_ptr<GraphicsPipeline>& pipeline)
		:	buffer(nullptr),
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
			std::cerr << "No reference to command pool provided.\nFailed to create CommandBuffer" << std::endl;
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
			std::cerr << "No reference to logical device.\nFailed to create command buffer." << std::endl;
			return;
		}
		const auto& device = logicalDevicePtr->Get();

		const auto result = vkAllocateCommandBuffers(device, &allocInfo, &buffer);
		S_ASSERT(result != VK_SUCCESS, "Failed to create commandBuffer!");
	}

	void CommandBuffer::Record(uint32_t imageIndex) const
	{
		const auto graphicsPipelinePtr = graphicsPipeline.lock();
		if (!graphicsPipelinePtr)
		{
			std::cerr << "Failed to create render pass as reference to graphics pipeline is null." << std::endl;
			return;
		}

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0;	// Optional
		beginInfo.pInheritanceInfo = nullptr;	// Optional (only relevant to secondary command buffer)

		const VkResult result = vkBeginCommandBuffer(buffer, &beginInfo);
		S_ASSERT(result != VK_SUCCESS, "Failed to begin recording command buffer!");

		StartRenderPass(imageIndex);

		graphicsPipelinePtr->BindGraphicsPipeline(*this);

		graphicsPipelinePtr->Draw(*this);

		EndRenderPass();

		End();
	}

	void CommandBuffer::Reset() const
	{
		vkResetCommandBuffer(buffer, 0);
	}

	void CommandBuffer::End() const
	{
		const auto result = vkEndCommandBuffer(buffer);
		S_ASSERT(result != VK_SUCCESS, "Faield to record command buffer!");
	}

	const VkCommandBuffer& CommandBuffer::GetBuffer() const
	{
		return buffer;
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
		vkCmdBeginRenderPass(buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void CommandBuffer::EndRenderPass() const
	{
		vkCmdEndRenderPass(buffer);
	}
}
