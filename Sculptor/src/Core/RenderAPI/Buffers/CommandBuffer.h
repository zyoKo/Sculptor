#pragma once
#include "Utilities/Macros.h"

namespace Sculptor::Core
{
	class LogicalDevice;
	class CommandPool;
	class RenderApi;
	class FrameBuffer;
	class SwapChain;
	class GraphicsPipeline;
}

namespace Sculptor::Core
{
	// Cleanup done when command pool is freed
	class CommandBuffer
	{
	public:
		CommandBuffer();

		CommandBuffer(
			std::weak_ptr<CommandPool>		commandPool, 
			std::weak_ptr<LogicalDevice>	device,
			std::weak_ptr<RenderApi>		renderApi, 
			std::weak_ptr<FrameBuffer>		buffer,
			std::weak_ptr<SwapChain>		swapChain, 
			std::weak_ptr<GraphicsPipeline> pipeline) noexcept;

		~CommandBuffer() = default;

		void Create();

		// imageIndex: index of current swap-chain image
		void Record(uint32_t imageIndex) const;

		void Reset() const;

		void End() const;

		static VkCommandBuffer BeginSingleTimeCommand(const VkCommandPool& commandPool, const VkDevice& device);

		static void EndSingleTimeCommand(const VkCommandBuffer& commandBuffer);

		VkCommandBuffer& GetBuffer();

		const VkCommandBuffer& GetBuffer() const;

		void SetCommandPool(std::weak_ptr<CommandPool> commandPool) noexcept;

		void SetFrameBuffer(std::weak_ptr<FrameBuffer> buffer) noexcept;

		void SetGraphicsPipeline(std::weak_ptr<GraphicsPipeline> pipeline) noexcept;

		LOGICAL_DEVICE

		RENDER_API

		SWAP_CHAIN

	private:
		VkCommandBuffer commandBuffer;

		std::weak_ptr<CommandPool> commandPool;

		std::weak_ptr<FrameBuffer> frameBuffer;

		std::weak_ptr<GraphicsPipeline> graphicsPipeline;

		// TODO: Maybe move it to the RenderApi class
		void StartRenderPass(uint32_t imageIndex) const;

		void EndRenderPass() const;
	};
}
