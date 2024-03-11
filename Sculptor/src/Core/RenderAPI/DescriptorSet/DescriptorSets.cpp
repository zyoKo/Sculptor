#include <SculptorPch.h>

#include "DescriptorSets.h"

#include "Core/Data/Constants.h"
#include "Utilities/Logger/Assert.h"
#include "Core/RenderAPI/Utility/CreateInfo.h"
#include "Core/Locators/DescriptorPoolLocator.h"
#include "Core/Locators/LogicalDeviceLocator.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Core/RenderAPI/Buffers/UniformBuffer.h"
#include "Core/RenderAPI/Buffers/Structures/UniformBufferObject.h"
#include "Core/RenderAPI/Image/TextureImageView.h"
#include "Core/RenderAPI/Image/Sampler/TextureSampler.h"

namespace Sculptor::Core
{
	void DescriptorSets::Allocate(const std::vector<std::weak_ptr<DescriptorSetLayout>>& descriptorSetLayouts,
			const std::vector<std::shared_ptr<UniformBuffer>>& uniformBuffers,
			const std::vector<std::tuple<TextureImageView, TextureSampler>>& textureDataList)
	{
		LOGICAL_DEVICE_LOCATOR

		std::vector<VkDescriptorSetLayout> layouts;
		layouts.reserve(descriptorSetLayouts.size());
		for (const auto& weakDescriptorSet : descriptorSetLayouts)
		{
			const auto descriptorSetLayoutPtr = weakDescriptorSet.lock();
			S_ASSERT(descriptorSetLayoutPtr == nullptr, "Failed to create descriptor set.")
			const auto& descriptorSetLayout = descriptorSetLayoutPtr->GetDescriptorSetLayout();

			layouts.emplace_back(descriptorSetLayout);
		}

		AllocateAndUpdateDescriptorSets(layouts, uniformBuffers, textureDataList);
	}

	void DescriptorSets::Allocate(const std::weak_ptr<DescriptorSetLayout>& weakDescriptorSetLayout,
			const std::vector<std::shared_ptr<UniformBuffer>>& uniformBuffers,
			const std::vector<std::tuple<TextureImageView, TextureSampler>>& textureDataList)
	{
		LOGICAL_DEVICE_LOCATOR

		const auto descriptorSetLayoutPtr = weakDescriptorSetLayout.lock();
		S_ASSERT(!descriptorSetLayoutPtr, "Descriptor Set Layout Ptr is null.")
		const auto descriptorSetLayout = descriptorSetLayoutPtr->GetDescriptorSetLayout();

		const std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);

		AllocateAndUpdateDescriptorSets(layouts, uniformBuffers, textureDataList);
	}

	const std::vector<VkDescriptorSet>& DescriptorSets::GetDescriptorSets() const
	{
		return descriptorSets;
	}

	void DescriptorSets::AllocateAndUpdateDescriptorSets(
		const std::vector<VkDescriptorSetLayout>& layouts,
		const std::vector<std::shared_ptr<UniformBuffer>>& uniformBuffers,
		const std::vector<std::tuple<TextureImageView, TextureSampler>>& textureDataList)
	{
		GetShared<LogicalDevice> logicalDevicePtr { LogicalDeviceLocator::GetLogicalDevice() };
		const auto& device = logicalDevicePtr->Get();

		GetShared<DescriptorPool> descriptorPoolPtr { DescriptorPoolLocator::GetDescriptorPool() };
		const auto descriptorPool = descriptorPoolPtr->GetDescriptorPool();

		const auto allocInfo = CreateInfo<VkDescriptorSetAllocateInfo>({
			.descriptorPool = descriptorPool,
			.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT),
			.pSetLayouts = layouts.data()
		});

		descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);

		VK_CHECK(vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()), "Failed to create descriptor sets.")

		for (unsigned i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			const auto& uniformBuffer = static_cast<VkBuffer>(*uniformBuffers[i]);

			// Uniform Buffer Info
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = uniformBuffer;
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			// Image Info
			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = std::get<0>(textureDataList[0]).GetImageView();
			imageInfo.sampler = std::get<1>(textureDataList[0]).GetTextureSampler();

			std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[0].dstSet = descriptorSets[i];
			descriptorWrites[0].dstBinding = 0;
			descriptorWrites[0].dstArrayElement = 0;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pBufferInfo = &bufferInfo;
			descriptorWrites[0].pImageInfo = nullptr;		// Optional
			descriptorWrites[0].pTexelBufferView = nullptr;	// Optional

			descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[1].dstSet = descriptorSets[i];
			descriptorWrites[1].dstBinding = 1;
			descriptorWrites[1].dstArrayElement = 0;
			descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[1].descriptorCount = 1;
			descriptorWrites[1].pImageInfo = &imageInfo;
			descriptorWrites[1].pBufferInfo = nullptr;		// Optional
			descriptorWrites[1].pTexelBufferView = nullptr;	// Optional

			vkUpdateDescriptorSets(device, 
				static_cast<U32>(descriptorWrites.size()), 
				descriptorWrites.data(), 
				0, 
				nullptr);
		}
	}
}
