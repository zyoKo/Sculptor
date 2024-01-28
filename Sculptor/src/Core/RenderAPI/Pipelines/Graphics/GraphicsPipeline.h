#pragma once

#include "Core/RenderAPI/Buffers/CommandBuffer.h"

namespace Sculptor::Core
{
	class VertexBuffer;
	class ShaderModule;
	class SwapChain;
	class RenderApi;
	class LogicalDevice;
	class IndexBuffer;
}

namespace Sculptor::Core
{
	class GraphicsPipeline
	{
	public:
		GraphicsPipeline() = default;

		GraphicsPipeline(const std::weak_ptr<RenderApi>& renderApi, const std::weak_ptr<SwapChain>& swapChain,
			const std::weak_ptr<LogicalDevice>& device);

		void CreateGraphicsPipeline();

		void CleanUp() const;

		void BindGraphicsPipeline(const CommandBuffer& commandBuffer) const;

		void Draw(const CommandBuffer& commandBuffer, uint32_t bufferSize) const;

		void SetRenderApi(const std::weak_ptr<RenderApi>& renderApi);

		void SetSwapChain(const std::weak_ptr<SwapChain>& swapChain);

		void SetLogicalDevice(const std::weak_ptr<LogicalDevice>& device);

		void SetVertexBuffer(const std::weak_ptr<VertexBuffer>& buffer);

		void SetIndexBuffer(const std::weak_ptr<IndexBuffer>& buffer);

	private:
		VkPipelineLayout pipelineLayout{};

		VkPipeline graphicsPipeline{};

		std::shared_ptr<ShaderModule> shaderModule;

		std::weak_ptr<RenderApi> renderApi;

		std::weak_ptr<SwapChain> swapChain;

		std::weak_ptr<LogicalDevice> logicalDevice;

		std::weak_ptr<VertexBuffer> vertexBuffer;

		std::weak_ptr<IndexBuffer> indexBuffer;
	};
}
