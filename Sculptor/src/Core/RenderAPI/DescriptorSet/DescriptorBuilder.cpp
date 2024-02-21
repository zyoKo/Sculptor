/*
 * Filename	: DescriptorBuilder.cpp
 * Author	: Vatsalya Yadav
 * Date		: 2-20-2024
 * Email	: vatsalya.yd@gmail.com
*/

#include <SculptorPch.h>

#include "DescriptorBuilder.h"

namespace Sculptor::Core
{
	/*! TODO: Fill this later after finishing Deferred shading
	* \brief Returning by reference and not const reference cause we want to be able to change data within VkDescriptorSetLayoutBinding vector
	* \param uniformBuffer	: choose uniform buffer to create descriptor set from
	* \param binding		: binding as specified in the shader
	* \param bufferRange	: 
	* \param offset			: 
	* \param stageFlag		: 
	* \return return reference to this class
	*/
	DescriptorBuilder& DescriptorBuilder::AddUniformBuffer(const VkBuffer uniformBuffer, U32 binding, VkDeviceSize bufferRange, 
		VkDeviceSize offset /* = 0 */,
		VkShaderStageFlags stageFlag /* = VK_SHADER_STAGE_VERTEX_BIT */)
	{
		layoutBindings.push_back({
			.binding = binding,
			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount = 1,
			.stageFlags = stageFlag,
			.pImmutableSamplers = nullptr
		});

		bufferInfos.push_back({
			.buffer = uniformBuffer,
			.offset = offset,
			.range = bufferRange
		});

		descriptorWrites.push_back({
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.pNext = nullptr,
			.dstSet = nullptr,
			.dstBinding = 0,
			.dstArrayElement = 0,
			.descriptorCount = 1,	// TODO: Manage array later
			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.pImageInfo = nullptr,
			.pBufferInfo = &bufferInfos.back(),
			.pTexelBufferView = nullptr
		});

		return *this;
	}

	/*!
	 * \brief 
	 * \param imageSampler 
	 * \param binding 
	 * \param imageView 
	 * \param imageLayout 
	 * \param stageFlag 
	 * \return 
	 */
	DescriptorBuilder& DescriptorBuilder::AddImageSampler(const VkSampler imageSampler, U32 binding, VkImageView imageView, 
		VkImageLayout imageLayout /* = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL */, 
		VkShaderStageFlags stageFlag /* = VK_SHADER_STAGE_FRAGMENT_BIT */)
	{
		layoutBindings.push_back({
			.binding = binding,
			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount = 1,
			.stageFlags = stageFlag,
			.pImmutableSamplers = nullptr
		});

		imageInfos.push_back({
			.sampler = imageSampler,
			.imageView = imageView,
			.imageLayout = imageLayout
		});

		descriptorWrites.push_back({
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.pNext = nullptr,
			.dstSet = nullptr,
			.dstBinding = 0,
			.dstArrayElement = 0,
			.descriptorCount = 1,	// TODO: Manage array later
			.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.pImageInfo = &imageInfos.back(),
			.pBufferInfo = nullptr,
			.pTexelBufferView = nullptr
		});

		return *this;
	}
}
