#include <SculptorPch.h>

#include "GraphicsPipeline.h"

#include "Core/RenderAPI/Data/Constants.h"
#include "Core/RenderAPI/Structures/Scissor.h"
#include "Core/RenderAPI/Structures/Viewport.h"
#include "Core/RenderAPI/RenderApi.h"
#include "Core/RenderAPI/SwapChains/SwapChain.h"
#include "Utilities/Logger/Assert.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Core/RenderAPI/Shader/ShaderModule.h"

namespace Sculptor::Core
{
	GraphicsPipeline::GraphicsPipeline(const std::weak_ptr<RenderApi>& renderApi, 
		const std::weak_ptr<SwapChain>& swapChain, 
		const std::weak_ptr<LogicalDevice>& device)
		:	renderApi(renderApi),
			swapChain(swapChain),
			logicalDevice(device)
	{
		shaderModule = std::make_shared<ShaderModule>(device);
	}

	void GraphicsPipeline::CreateGraphicsPipeline()
	{
		shaderModule->CreateShaderStages();

		VkPipelineVertexInputStateCreateInfo vertexInput{};
		vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInput.vertexBindingDescriptionCount = 0;
		vertexInput.pVertexBindingDescriptions = nullptr; // Optional
		vertexInput.vertexAttributeDescriptionCount = 0;
		vertexInput.pVertexAttributeDescriptions = nullptr; // Optional

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;

		// Following are the possible topologies:
		// - VK_PRIMITIVE_TOPOLOGY_POINT_LIST
		// - VK_PRIMITIVE_TOPOLOGY_LINE_LIST
		// - VK_PRIMITIVE_TOPOLOGY_LINE_STRIP // only when primitiveRestartEnable flag is set to VK_TRUE
		// - VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST // probably used below
		// - VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		// with VK_TRUE here we could break up lines and triangles into _STRIP topology
		// topology modes uses special index of '0xFFFF' or '0xFFFFFFFF'.
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(DYNAMIC_STATES.size());
		dynamicState.pDynamicStates = DYNAMIC_STATES.data();

		const auto swapChainPtr = swapChain.lock();
		if (!swapChainPtr)
		{
			std::cerr << "SwapChain is null while creating graphics pipeline!" << std::endl;
			return;
		}

		const Viewport viewPort(swapChainPtr->swapChainExtent);
		const Scissor scissor(swapChainPtr->swapChainExtent);

		// Info: Using multiple viewport can only be possible if the GPU supports it
		//       This needs to be enabled while creating LogicalDevice creation process
		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewPort.vkViewPort;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor.scissor;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;

		// VK_TRUE then fragments that are beyond the near and far planes are clamped and discarded beyond this range
		rasterizer.depthClampEnable = VK_FALSE;

		// Types of polygonModes:
		// - VK_POLYGON_MODE_POINT
		// - VK_POLYGON_MODE_LINE
		// ...
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;

		// if set to VK_TRUE then geometry never passes through rasterization phase,
		// basically disables any output the frame-buffer
		rasterizer.rasterizerDiscardEnable = VK_FALSE;

		// 
		rasterizer.lineWidth = DEFAULT_LINE_WIDTH_FOR_RASTERIZER;

		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;

		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f; // Optional
		rasterizer.depthBiasClamp = 0.0f; // Optional
		rasterizer.depthBiasSlopeFactor = 0.0f;	// Optional

		// Multi-Sampling phase (later)
		VkPipelineMultisampleStateCreateInfo multiSampling{};
		multiSampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multiSampling.sampleShadingEnable = VK_FALSE;
		multiSampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multiSampling.minSampleShading = 1.0f; // Optional
		multiSampling.pSampleMask = nullptr; // Optional
		multiSampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multiSampling.alphaToOneEnable = VK_FALSE; // Optional

		// Depth and Stencil (later)
		//VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo{};
		//depthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = 
			VK_COLOR_COMPONENT_R_BIT | 
			VK_COLOR_COMPONENT_G_BIT | 
			VK_COLOR_COMPONENT_B_BIT | 
			VK_COLOR_COMPONENT_A_BIT;

		// since it's VK_FALSE no blending is done
		// else the following pseudo code happens
		// if (blendEnable)
		// {
		//		finalColor.rgb = (srcColorBlendFactor * newColor.rgb) <colorBlendOp> (dstColorBlendFactor * oldColor.rgb);
		//		finalColor.a = (srcAlphaBlendFactor * newColor.a) <alphaBlendOp> (dstAlphaBlendFactor * oldColor.a);
	    // }
	    // else
	    // {
		//		finalColor = newColor;
		// }
		//
		// finalColor = finalColor & colorWriteMask;
		colorBlendAttachment.blendEnable = VK_FALSE;

		// Optional
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		// alpha blending can be achieved as this
		// colorBlendAttachment.blendEnable = VK_TRUE;
		// colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		// colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		// colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		// colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		// colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		// colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		// this operation can also be achieved using VkBlendFactor or VkBlendOp enumerations in the specification

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;

		// Optional
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		// Finally Creating Pipeline Layout
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0; // Optional
		pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

		VkGraphicsPipelineCreateInfo graphicsPipelineInfo{};
		graphicsPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		graphicsPipelineInfo.stageCount = 2;
		graphicsPipelineInfo.pStages = shaderModule->shaderStages.data();
		graphicsPipelineInfo.pVertexInputState = &vertexInput;
		graphicsPipelineInfo.pInputAssemblyState = &inputAssembly;
		graphicsPipelineInfo.pViewportState = &viewportState;
		graphicsPipelineInfo.pRasterizationState = &rasterizer;
		graphicsPipelineInfo.pMultisampleState = &multiSampling;
		graphicsPipelineInfo.pDepthStencilState = nullptr; // Optional (later)
		graphicsPipelineInfo.pColorBlendState = &colorBlending;
		graphicsPipelineInfo.pDynamicState = &dynamicState;
		graphicsPipelineInfo.layout = pipelineLayout;

		const auto renderApiPtr = renderApi.lock();
		if (!renderApiPtr)
		{
			std::cerr << "Initialize and Set Render Api before creating graphics pipeline." << std::endl;
			return;
		}

		graphicsPipelineInfo.renderPass = renderApiPtr->renderPass;
		graphicsPipelineInfo.subpass = 0;
		graphicsPipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		graphicsPipelineInfo.basePipelineIndex = -1; // Optional

		const auto& logicalDevicePtr = logicalDevice.lock();
		S_ASSERT(!logicalDevicePtr, "Failed to create shader module.");

		const auto& device = logicalDevicePtr->GetLogicalDevice();

		const auto result = vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout);
		S_ASSERT(result != VK_SUCCESS, "Failed to create Pipeline Layout!");

		shaderModule->DestroyShaderModules();
	}

	void GraphicsPipeline::CleanUp() const
	{
		const auto& logicalDevicePtr = logicalDevice.lock();
		S_ASSERT(!logicalDevicePtr, "Failed to create shader module.");

		const auto& device = logicalDevicePtr->GetLogicalDevice();

		vkDestroyPipeline(device, graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	}

	void GraphicsPipeline::SetRenderApi(const std::weak_ptr<RenderApi>& renderApi)
	{
		this->renderApi = renderApi;
	}

	void GraphicsPipeline::SetSwapChain(const std::weak_ptr<SwapChain>& swapChain)
	{
		this->swapChain = swapChain;
	}

	void GraphicsPipeline::SetLogicalDevice(const std::weak_ptr<LogicalDevice>& device)
	{
		this->logicalDevice = device;
	}
}
