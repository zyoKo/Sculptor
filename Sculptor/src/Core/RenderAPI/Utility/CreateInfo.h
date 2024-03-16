#pragma once

namespace Sculptor::Core
{
	/*!
	 * \brief Creates Info type based on template specialization provided
	 * \tparam T : is any info type parameter
	 * \param pNextChain : pointer to pNext for each info
	 * \return returns the Info type based on template specialization
	 */
	template <typename T>
	T CreateInfo(T = {}, const void* pNextChain = VK_NULL_HANDLE);

	template <>
	[[nodiscard]] inline VkDeviceCreateInfo CreateInfo(VkDeviceCreateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		info.pNext = pNextChain;
		info.flags = 0;
		return info;
	}

	template <>
	[[nodiscard]] inline VkDeviceQueueCreateInfo CreateInfo(VkDeviceQueueCreateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		info.pNext = pNextChain;
		info.flags = 0;
		return info;
	}

	template <>
	[[nodiscard]] inline VkApplicationInfo CreateInfo(VkApplicationInfo info, const void* pNextChain)
	{
	    info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		info.pNext = pNextChain;
	    return info;
	}

	template <>
	[[nodiscard]] inline VkInstanceCreateInfo CreateInfo(VkInstanceCreateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		info.pNext = pNextChain;
		return info;
	}

	template <>
	[[nodiscard]] inline VkDescriptorSetAllocateInfo CreateInfo(VkDescriptorSetAllocateInfo info, const void* pNextChain /* = VK_NULL_HANDLE */)
	{
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		info.pNext = pNextChain;
		return info;
	}

	template <>
	[[nodiscard]] inline VkPipelineShaderStageCreateInfo CreateInfo(VkPipelineShaderStageCreateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		info.pNext = pNextChain;
		return info;
	}

	template <>
	[[nodiscard]] inline VkGraphicsPipelineCreateInfo CreateInfo(VkGraphicsPipelineCreateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		info.pNext = pNextChain;
		return info;
	}

	template <>
	[[nodiscard]] inline VkPipelineLayoutCreateInfo CreateInfo(VkPipelineLayoutCreateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		info.pNext = pNextChain;
		return info;
	}

	template <>
	[[nodiscard]] inline VkRenderPassBeginInfo CreateInfo(VkRenderPassBeginInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		info.pNext = pNextChain;
		return info;
	}

	template <>
	[[nodiscard]] inline VkCommandBufferBeginInfo CreateInfo(VkCommandBufferBeginInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.pNext = pNextChain;
		info.flags = 0;
		info.pInheritanceInfo = VK_NULL_HANDLE;
		return info;
	}

	template <>
	[[nodiscard]] inline VkCommandBufferAllocateInfo CreateInfo(VkCommandBufferAllocateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.pNext = pNextChain;
		return info;
	}

	template <>
	[[nodiscard]] inline VkSubmitInfo CreateInfo(VkSubmitInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.pNext = pNextChain;
		return info;
	}

	template <>
	[[nodiscard]] inline VkPresentInfoKHR CreateInfo(VkPresentInfoKHR info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		info.pNext = pNextChain;
		return info;
	}

	template <>
	[[nodiscard]] inline VkPipelineDynamicStateCreateInfo CreateInfo(VkPipelineDynamicStateCreateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		info.pNext = pNextChain;
		info.flags = 0;
		return info;
	}

	/*
	 * Info: Using multiple viewport can only be possible if the GPU supports it
	 *       This needs to be enabled while creating LogicalDevice creation process
	 */
	template <>
	[[nodiscard]] inline VkPipelineViewportStateCreateInfo CreateInfo(VkPipelineViewportStateCreateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		info.pNext = pNextChain;
		return info;
	}

	/*
	 * Info:
	 * if (depthClampEnable) {
	 *     then fragments beyond the near and far planes are clamped and discarded beyond this range
	 * }
	 * if (rasterizerDiscardEnable) {
	 *	   then geometry never passes through rasterization phase, basically disables any output the frame-buffer
	 * }
	 */
	template <>
	[[nodiscard]] inline VkPipelineRasterizationStateCreateInfo CreateInfo(VkPipelineRasterizationStateCreateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		info.pNext = pNextChain;
		info.flags = 0;
		info.depthClampEnable = VK_FALSE; 
		info.rasterizerDiscardEnable = VK_FALSE;
		info.depthBiasEnable = VK_FALSE;
		info.depthBiasConstantFactor = 0.0f; // Optional
		info.depthBiasClamp			 = 0.0f; // Optional
		info.depthBiasSlopeFactor	 = 0.0f; // Optional
		return info;
	}

	template <>
	[[nodiscard]] inline VkPipelineMultisampleStateCreateInfo CreateInfo(VkPipelineMultisampleStateCreateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		info.pNext = pNextChain;
		info.flags = 0;
		info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		info.sampleShadingEnable = VK_FALSE;
		info.minSampleShading = 1.0f;			// Optional
		info.pSampleMask = VK_NULL_HANDLE;		// Optional
		info.alphaToCoverageEnable = VK_FALSE;  // Optional
		return info;
	}

	template <>
	[[nodiscard]] inline VkPipelineDepthStencilStateCreateInfo CreateInfo(VkPipelineDepthStencilStateCreateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		info.pNext = pNextChain;
		return info;
	}

	/*
	 * Following are the possible topologies:
	 * - VK_PRIMITIVE_TOPOLOGY_POINT_LIST
	 * - VK_PRIMITIVE_TOPOLOGY_LINE_LIST
	 * - VK_PRIMITIVE_TOPOLOGY_LINE_STRIP // only when 'primitiveRestartEnable' flag is set to VK_TRUE
	 * - VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
	 * - VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP
	 * ----------------------------------------
	 * if (primitiveRestartEnable = VK_TRUE)
	 * {
	 *	  here we could break up lines and triangles into _STRIP topology modes
	 *    which uses special index of '0xFFFF' or '0xFFFFFFFF'.
	 * }
	 */
	template <>
	[[nodiscard]] inline VkPipelineInputAssemblyStateCreateInfo CreateInfo(VkPipelineInputAssemblyStateCreateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		info.pNext = pNextChain;
		info.flags = 0;
		return info;
	}

	template <>
	[[nodiscard]] inline VkPipelineVertexInputStateCreateInfo CreateInfo(VkPipelineVertexInputStateCreateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		info.pNext = pNextChain;
		info.flags = 0;
		return info;
	}

	/*
	 *	alpha blending can be achieved as this
	 *	colorBlendAttachment.blendEnable = VK_TRUE;
	 *	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	 *	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	 *	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	 *	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	 *	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	 *	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	 *	this operation can also be achieved using VkBlendFactor or VkBlendOp enumerations in the specification
	 */
	template <>
	[[nodiscard]] inline VkPipelineColorBlendStateCreateInfo CreateInfo(VkPipelineColorBlendStateCreateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		info.pNext = pNextChain;
		info.flags = 0;
		info.logicOpEnable = VK_FALSE;
		info.logicOp = VK_LOGIC_OP_COPY;
		info.blendConstants[0] = 0.0f;
		info.blendConstants[1] = 0.0f;
		info.blendConstants[2] = 0.0f;
		info.blendConstants[3] = 0.0f;
		return info;
	}

	template <>
	[[nodiscard]] inline VkRenderPassCreateInfo CreateInfo(VkRenderPassCreateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		info.pNext = pNextChain;
		info.flags = 0;
		return info;
	}

	template <>
	[[nodiscard]] inline VkImageCreateInfo CreateInfo(VkImageCreateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		info.pNext = pNextChain;
		info.flags = 0;
		return info;
	}

	template <>
	[[nodiscard]] inline VkMemoryAllocateInfo CreateInfo(VkMemoryAllocateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		info.pNext = pNextChain;
		return info;
	}

	template <>
	[[nodiscard]] inline VkImageViewCreateInfo CreateInfo(VkImageViewCreateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		info.pNext = pNextChain;
		info.flags = 0;
		return info;
	}

	template <>
	[[nodiscard]] inline VkFramebufferCreateInfo CreateInfo(VkFramebufferCreateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		info.pNext = pNextChain;
		info.flags = 0;
		return info;
	}

	template <>
	[[nodiscard]] inline VkDescriptorSetLayoutCreateInfo CreateInfo(VkDescriptorSetLayoutCreateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		info.pNext = pNextChain;
		info.flags = 0;
		return info;
	}

	template <>
	[[nodiscard]] inline VkDescriptorPoolCreateInfo CreateInfo(VkDescriptorPoolCreateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		info.pNext = pNextChain;
		info.flags = 0;
		return info;
	}

	template <>
	[[nodiscard]] inline VkSamplerCreateInfo CreateInfo(VkSamplerCreateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		info.pNext = pNextChain;
		info.flags = 0;
		return info;
	}

	template <>
	[[nodiscard]] inline VkCommandPoolCreateInfo CreateInfo(VkCommandPoolCreateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		info.pNext = pNextChain;
		info.flags = 0;
		return info;
	}

	template <>
	[[nodiscard]] inline VkBufferCreateInfo CreateInfo(VkBufferCreateInfo info, const void* pNextChain)
	{
		info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		info.pNext = pNextChain;
		info.flags = 0;
		return info;
	}
}