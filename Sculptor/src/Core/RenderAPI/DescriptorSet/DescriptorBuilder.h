#pragma once

#include "ResourceBuilder.h"

namespace Sculptor::Core
{
	class DescriptorBuilder
	{
	public:
		DescriptorBuilder(std::weak_ptr<LogicalDevice> logicalDevice) noexcept;

		DescriptorBuilder& AddUniformBuffer(const VkBuffer uniformBuffer, U32 binding, VkDeviceSize bufferRange,
			VkDeviceSize offset = 0,
			VkShaderStageFlags stageFlag = VK_SHADER_STAGE_VERTEX_BIT);

		DescriptorBuilder& AddImageSampler(const VkSampler imageSampler, U32 binding, VkImageView imageView, 
			VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 
			VkShaderStageFlags stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT);

		ResourceBuilder Build(std::string name);

		LOGICAL_DEVICE

	private:
		std::vector<VkDescriptorSetLayoutBinding> layoutBindings;

		std::vector<VkWriteDescriptorSet> descriptorWrites;

		std::vector<VkDescriptorBufferInfo> bufferInfos;

		std::vector<VkDescriptorImageInfo> imageInfos;
	};
}
