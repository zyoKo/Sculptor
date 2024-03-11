#include <SculptorPch.h>

#include "GraphicsPipeline.h"

#include "Core/Core.h"
#include "Core/Data/Constants.h"
#include "Structures/Scissor.h"
#include "Structures/Viewport.h"
#include "Core/RenderAPI/RenderApi.h"
#include "Core/RenderAPI/Buffers/Structures/Vertex.h"
#include "Core/RenderAPI/SwapChains/SwapChain.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Core/RenderAPI/Shader/ShaderModule.h"
#include "Core/RenderAPI/Buffers/VertexBuffer.h"
#include "Core/RenderAPI/Buffers/IndexBuffer.h"
#include "Core/RenderAPI/Buffers/Data/Constants.h"
#include "Core/RenderAPI/DescriptorSet/DescriptorSetLayout.h"
#include "Core/RenderAPI/DescriptorSet/DescriptorSets.h"
#include "Core/RenderAPI/Utility/CreateInfo.h"

namespace Sculptor::Core
{
	GraphicsPipeline::GraphicsPipeline()
		:	pipelineLayout(VK_NULL_HANDLE),
			graphicsPipeline(VK_NULL_HANDLE),
			currentFrame(0)
	{ }

	GraphicsPipeline::GraphicsPipeline(std::weak_ptr<RenderApi> renderApi, 
	                                   std::weak_ptr<SwapChain> swapChain, 
	                                   std::weak_ptr<LogicalDevice> device) noexcept
		:	logicalDevice(std::move(device)),
			swapChain(std::move(swapChain)),
			renderApi(std::move(renderApi)),
			pipelineLayout(VK_NULL_HANDLE),
			graphicsPipeline(VK_NULL_HANDLE),
			shaderModule(std::make_shared<ShaderModule>(logicalDevice)),
			currentFrame(0)
	{ }

	void GraphicsPipeline::Create()
	{
		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto& device = logicalDevicePtr->Get();

		GetShared<RenderApi> renderApiPtr{ renderApi };
		const auto renderPass = renderApiPtr->renderPass;

		GetShared<SwapChain> swapChainPtr { swapChain };
		const Viewport viewPort(swapChainPtr->swapChainExtent);
		const Scissor scissor(swapChainPtr->swapChainExtent);

		shaderModule->CreateShaderStages();

		auto bindingDesc = Vertex::GetBindingDescription(0, sizeof(Vertex));
		auto attributeDesc = Vertex::GetAttributeDescription();

		const auto vertexInput = CreateInfo<VkPipelineVertexInputStateCreateInfo>({
			.vertexBindingDescriptionCount = 1,
			.pVertexBindingDescriptions = &bindingDesc,
			.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDesc.size()),
			.pVertexAttributeDescriptions = attributeDesc.data()
		});

		const auto inputAssembly = CreateInfo<VkPipelineInputAssemblyStateCreateInfo>();

		const auto dynamicStateInfo = CreateInfo<VkPipelineDynamicStateCreateInfo>({
			.dynamicStateCount = static_cast<uint32_t>(DYNAMIC_STATES.size()),
			.pDynamicStates = DYNAMIC_STATES.data()
		});

		const auto viewportState = CreateInfo<VkPipelineViewportStateCreateInfo>({
			.viewportCount = 1,
			.pViewports = &viewPort.vkViewPort,
			.scissorCount = 1,
			.pScissors = &scissor.scissor
		});

		const auto rasterizer = CreateInfo<VkPipelineRasterizationStateCreateInfo>({
			.polygonMode = VK_POLYGON_MODE_FILL,
			.cullMode = VK_CULL_MODE_BACK_BIT,
			.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
			.lineWidth = DEFAULT_LINE_WIDTH_FOR_RASTERIZER,
		});

		// Multi-Sampling phase (later)
		const auto multiSampling = CreateInfo<VkPipelineMultisampleStateCreateInfo>();

		// Depth and Stencil (later)
		const auto depthStencil = CreateInfo<VkPipelineDepthStencilStateCreateInfo>({
			.depthTestEnable = VK_TRUE,
			.depthWriteEnable = VK_TRUE,
			.depthCompareOp = VK_COMPARE_OP_LESS,
			.depthBoundsTestEnable = VK_FALSE,
			.stencilTestEnable = VK_FALSE,
			.front = {},
			.back = {},
			.minDepthBounds = 0.0f,
			.maxDepthBounds = 1.0f
		});

		constexpr VkPipelineColorBlendAttachmentState colorBlendAttachment{
			.blendEnable = VK_FALSE,
			.srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
			.colorBlendOp		 = VK_BLEND_OP_ADD,
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
			.alphaBlendOp		 = VK_BLEND_OP_ADD,
			.colorWriteMask =
				VK_COLOR_COMPONENT_R_BIT | 
				VK_COLOR_COMPONENT_G_BIT | 
				VK_COLOR_COMPONENT_B_BIT | 
				VK_COLOR_COMPONENT_A_BIT,
		};

		const auto colorBlending = CreateInfo<VkPipelineColorBlendStateCreateInfo>({
			.attachmentCount = 1,
			.pAttachments = &colorBlendAttachment
		});

		// Finally Creating Pipeline Layout
		GetShared<DescriptorSetLayout> descriptorSetLayoutPtr{ descriptorSetLayout };
		uint32_t setLayoutCount = 0;
		const VkDescriptorSetLayout* newDescriptorSet{ VK_NULL_HANDLE };
		if (descriptorSetLayoutPtr.IsValid())
		{
			setLayoutCount = 1;
			newDescriptorSet = descriptorSetLayoutPtr->GetDescriptorSetLayoutPointer();
		}

		const auto pipelineLayoutInfo = CreateInfo<VkPipelineLayoutCreateInfo>({
			.setLayoutCount = setLayoutCount,
			.pSetLayouts = newDescriptorSet,
			.pushConstantRangeCount = 0,
			.pPushConstantRanges = VK_NULL_HANDLE
		});

		VK_CHECK(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout), "Failed to create Pipeline Layout!")

		const auto graphicsPipelineInfo = CreateInfo<VkGraphicsPipelineCreateInfo>({
			.stageCount = 2,
			.pStages = shaderModule->shaderStages.data(),
			.pVertexInputState = &vertexInput,
			.pInputAssemblyState = &inputAssembly,
			.pViewportState = &viewportState,
			.pRasterizationState = &rasterizer,
			.pMultisampleState = &multiSampling,
			.pDepthStencilState = &depthStencil, // Optional (later)
			.pColorBlendState = &colorBlending,
			.pDynamicState = &dynamicStateInfo,
			.layout = pipelineLayout,
			.renderPass = renderPass,
			.subpass = 0,
			.basePipelineHandle = VK_NULL_HANDLE, // Optional
			.basePipelineIndex = -1 // Optional
		});

		VK_CHECK(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &graphicsPipelineInfo, nullptr, &graphicsPipeline), "Failed to create graphics pipeline.")

		shaderModule->DestroyShaderModules();
	}

	void GraphicsPipeline::CleanUp() const
	{
		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto& device = logicalDevicePtr->Get();

		vkDestroyPipeline(device, graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	}

	void GraphicsPipeline::BindGraphicsPipeline(const CommandBuffer& commandBuffer) const
	{
		vkCmdBindPipeline(commandBuffer.GetBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
	}

	void GraphicsPipeline::Draw(const CommandBuffer& commandBuffer) const
	{
		const auto& cmdBuffer = commandBuffer.GetBuffer();

		GetShared<VertexBuffer> vertexBufferPtr{ vertexBuffer };
		vertexBufferPtr->BindBuffer(cmdBuffer);

		GetShared<IndexBuffer> indexBufferPtr{ indexBuffer };
		indexBufferPtr->BindBuffer(cmdBuffer);

		GetShared<SwapChain> swapChainPtr{ swapChain };
		const auto& swapChainExtent = swapChainPtr->swapChainExtent;

		GetShared<DescriptorSets> descriptorSetPtr{ descriptorSets };
		const auto& descSets = descriptorSetPtr->GetDescriptorSets();

		const Viewport viewPort{ swapChainExtent };
		vkCmdSetViewport(cmdBuffer, 0, 1, &viewPort.vkViewPort);

		const Scissor scissor{ swapChainExtent };
		vkCmdSetScissor(cmdBuffer, 0, 1, &scissor.scissor);

		vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descSets[currentFrame], 0, nullptr);

		//vkCmdDrawIndexed(cmdBuffer, static_cast<uint32_t>(INDICES.size()), 1, 0, 0, 0);
		vkCmdDrawIndexed(cmdBuffer, 11484, 1, 0, 0, 0);
	}

	void GraphicsPipeline::SetCurrentFrame(U32 newFrame) noexcept
	{
		currentFrame = newFrame;
	}

	void GraphicsPipeline::SetVertexBuffer(std::weak_ptr<VertexBuffer> buffer) noexcept
	{
		this->vertexBuffer = std::move(buffer);
	}

	void GraphicsPipeline::SetIndexBuffer(std::weak_ptr<IndexBuffer> buffer) noexcept
	{
		this->indexBuffer = std::move(buffer);
	}

	void GraphicsPipeline::SetDescriptorSetLayout(std::weak_ptr<DescriptorSetLayout> descriptorSetLayout) noexcept
	{
		this->descriptorSetLayout = std::move(descriptorSetLayout);
	}

	void GraphicsPipeline::SetDescriptorSets(std::weak_ptr<DescriptorSets> descriptorSets) noexcept
	{
		this->descriptorSets = std::move(descriptorSets);
	}
}
