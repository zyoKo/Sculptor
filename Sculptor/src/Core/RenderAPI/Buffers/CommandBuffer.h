#pragma once

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
		CommandBuffer() = default;

		CommandBuffer(const std::weak_ptr<CommandPool>& commandPool, const std::weak_ptr<LogicalDevice>& device,
			const std::weak_ptr<RenderApi>& renderApi, const std::weak_ptr<FrameBuffer>& buffer,
			const std::weak_ptr<SwapChain>& swapChain, const std::weak_ptr<GraphicsPipeline>& pipeline);

		~CommandBuffer() = default;

		void Create();

		// imageIndex: index of current swap-chain image
		void Record(uint32_t imageIndex) const;

		void Reset() const;

		void End() const;

		const VkCommandBuffer& GetBuffer() const;

		void SetCommandPool(const std::weak_ptr<CommandPool>& commandPool);

		void SetLogicalDevice(const std::weak_ptr<LogicalDevice>& device);

		void SetRenderApi(const std::weak_ptr<RenderApi>& renderApi);

		void SetFrameBuffer(const std::weak_ptr<FrameBuffer>& buffer);

		void SetSwapChain(const std::weak_ptr<SwapChain>& swapChain);

		void SetGraphicsPipeline(const std::weak_ptr<GraphicsPipeline>& pipeline);

	private:
		VkCommandBuffer commandBuffer;

		std::weak_ptr<CommandPool> commandPool;

		std::weak_ptr<LogicalDevice> logicalDevice;

		std::weak_ptr<RenderApi> renderApi;

		std::weak_ptr<FrameBuffer> frameBuffer;

		std::weak_ptr<SwapChain> swapChain;

		std::weak_ptr<GraphicsPipeline> graphicsPipeline;

		// TODO: Maybe move it to the RenderApi class
		void StartRenderPass(uint32_t imageIndex) const;

		void EndRenderPass() const;
	};
}
