#pragma once

#include "Core/RenderAPI/Buffers/CommandBuffer.h"
#include "Utilities/Macros.h"

namespace Sculptor::Core
{
	class VertexBuffer;
	class ShaderModule;
	class SwapChain;
	class RenderApi;
	class LogicalDevice;
	class IndexBuffer;
	class DescriptorSetLayout;
	class DescriptorSets;
}

namespace Sculptor::Core
{
	class GraphicsPipeline
	{
	public:
		GraphicsPipeline();

		GraphicsPipeline(std::weak_ptr<RenderApi> renderApi, std::weak_ptr<SwapChain> swapChain, std::weak_ptr<LogicalDevice> device) noexcept;

		void Create();

		void CleanUp() const;

		void BindGraphicsPipeline(const CommandBuffer& commandBuffer) const;

		void Draw(const CommandBuffer& commandBuffer) const;

		void SetCurrentFrame(U32 newFrame) noexcept;

		void SetVertexBuffer(std::weak_ptr<VertexBuffer> buffer) noexcept;

		void SetIndexBuffer(std::weak_ptr<IndexBuffer> buffer) noexcept;

		//void SetDescriptorSetLayout(std::weak_ptr<DescriptorSetLayout> descriptorSetLayout) noexcept;

		//void SetDescriptorSets(std::weak_ptr<DescriptorSets> descriptorSets) noexcept;

		std::vector<VkDescriptorSet> descriptorSetsTest;

		VkDescriptorSetLayout descriptorSetLayoutTest;

		LOGICAL_DEVICE

		SWAP_CHAIN

		RENDER_API

	private:
		VkPipelineLayout pipelineLayout;

		VkPipeline graphicsPipeline;

		std::shared_ptr<ShaderModule> shaderModule;

		std::weak_ptr<VertexBuffer> vertexBuffer;

		std::weak_ptr<IndexBuffer> indexBuffer;

		//std::weak_ptr<DescriptorSetLayout> descriptorSetLayout;

		//std::weak_ptr<DescriptorSets> descriptorSets;

		uint32_t currentFrame;
	};
}
