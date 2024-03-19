/*
 * Filename	: DescriptorBuilder.cpp
 * Author	: Vatsalya Yadav
 * Date		: 2-20-2024
 * Email	: vatsalya.yd@gmail.com
*/

#include <SculptorPch.h>

#include "DescriptorBuilder.h"

#include "Core/Data/Constants.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"

namespace Sculptor::Core
{
	DescriptorBuilder::DescriptorBuilder(std::weak_ptr<LogicalDevice>&& logicalDevice) noexcept
		:	logicalDevice(logicalDevice)
	{ }

	/*! TODO: Fill this later after finishing Deferred shading
	* \brief Returning by reference and not const reference cause we want to be able to change data within VkDescriptorSetLayoutBinding vector
	* \param uniformBuffer	: choose uniform buffer to create descriptor set from
	* \param binding		: binding as specified in the shader
	* \param bufferRange	: 
	* \param offset			: 
	* \param stageFlag		: 
	* \return return reference to this class
	*/
	DescriptorBuilder& DescriptorBuilder::AddUniformBuffer(const std::vector<VkBuffer>& uniformBuffer, U32 binding, VkDeviceSize bufferRange, 
														   VkShaderStageFlags stageFlag /* = VK_SHADER_STAGE_VERTEX_BIT */, 
														   VkDeviceSize offset /* = 0 */)
	{
		layoutBindings.push_back({
			.binding			= binding,
			.descriptorType		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount	= 1,
			.stageFlags			= stageFlag,
			.pImmutableSamplers = VK_NULL_HANDLE
		});

		const auto currentSize = bufferInfos.size();
		const auto newSize = currentSize + uniformBuffer.size();

		bufferInfos.reserve(newSize);
		descriptorWrites.reserve(newSize);

		for (const auto uBuffer : uniformBuffer)
		{
			bufferInfos.emplace_back(VkDescriptorBufferInfo{
				.buffer		= uBuffer,
				.offset		= offset,
				.range		= bufferRange
			});

			descriptorWrites.emplace_back(VkWriteDescriptorSet{
				.sType				= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.pNext				= VK_NULL_HANDLE,
				.dstSet				= VK_NULL_HANDLE,
				.dstBinding			= 0,	// To be set later in the Resource Builder
				.dstArrayElement	= 0,
				.descriptorCount	= 1,	// TODO: Manage array later
				.descriptorType		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.pImageInfo			= VK_NULL_HANDLE,
				.pBufferInfo		= &bufferInfos.back(),
				.pTexelBufferView	= VK_NULL_HANDLE
			});
		}

		return *this;
	}

	/*!
	 * \brief 
	 * \param imageSampler	: 
	 * \param binding		: 
	 * \param imageView		: 
	 * \param imageLayout	: 
	 * \param stageFlag		: 
	 * \return 
	 */
	DescriptorBuilder& DescriptorBuilder::AddImageSampler(VkSampler imageSampler, U32 binding, VkImageView imageView, 
														  VkImageLayout imageLayout /* = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL */, 
														  VkShaderStageFlags stageFlag /* = VK_SHADER_STAGE_FRAGMENT_BIT */)
	{
		layoutBindings.push_back({
			.binding			= binding,
			.descriptorType		= VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.descriptorCount	= 1,
			.stageFlags			= stageFlag,
			.pImmutableSamplers = VK_NULL_HANDLE
		});

		const auto currentSize = imageInfos.size();
		const auto newSize = currentSize + MAX_FRAMES_IN_FLIGHT;

		imageInfos.reserve(newSize);
		descriptorWrites.reserve(newSize);

		for (unsigned i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			imageInfos.emplace_back(VkDescriptorImageInfo{
				.sampler = imageSampler,
				.imageView = imageView,
				.imageLayout = imageLayout
			});

			descriptorWrites.emplace_back(VkWriteDescriptorSet{
				.sType				= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.pNext				= VK_NULL_HANDLE,
				.dstSet				= VK_NULL_HANDLE,
				.dstBinding			= 0,	// To be set later in the Resource Builder
				.dstArrayElement	= 0,
				.descriptorCount	= 1,	// TODO: Manage array later
				.descriptorType		= VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.pImageInfo			= &imageInfos.back(),
				.pBufferInfo		= VK_NULL_HANDLE,
				.pTexelBufferView	= VK_NULL_HANDLE
			});
		}

		return *this;
	}

	ResourceBuilder DescriptorBuilder::Build(std::string name)
	{
		ResourceBuilder resourceBuilder(logicalDevice, std::move(name), std::move(layoutBindings), std::move(descriptorWrites));

		bufferInfos.clear();
		imageInfos.clear();

		return resourceBuilder;
	}
}
