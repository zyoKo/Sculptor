#include <SculptorPch.h>

#include "DescriptorSets.h"

#include "Core/Locators/DescriptorPoolLocator.h"
#include "Core/Locators/LogicalDeviceLocator.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Core/RenderAPI/Buffers/UniformBuffer.h"
#include "Core/RenderAPI/Buffers/Structures/UniformBufferObject.h"
#include "Core/RenderAPI/Data/Constants.h"
#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	void DescriptorSets::Allocate(const std::vector<std::weak_ptr<DescriptorSetLayout>>& descriptorSetLayouts,
		const std::vector<std::shared_ptr<UniformBuffer>>& uniformBuffers)
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

		AllocateAndUpdateDescriptorSets(layouts, uniformBuffers);
	}

	void DescriptorSets::Allocate(const std::weak_ptr<DescriptorSetLayout>& weakDescriptorSetLayout,
		const std::vector<std::shared_ptr<UniformBuffer>>& uniformBuffers)
	{
		LOGICAL_DEVICE_LOCATOR

		const auto descriptorSetLayoutPtr = weakDescriptorSetLayout.lock();
		S_ASSERT(!descriptorSetLayoutPtr, "Descriptor Set Layout Ptr is null.")
		const auto descriptorSetLayout = descriptorSetLayoutPtr->GetDescriptorSetLayout();

		const std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);

		AllocateAndUpdateDescriptorSets(layouts, uniformBuffers);
	}

	const std::vector<VkDescriptorSet>& DescriptorSets::GetDescriptorSets() const
	{
		return descriptorSets;
	}

	void DescriptorSets::AllocateAndUpdateDescriptorSets(
		const std::vector<VkDescriptorSetLayout>& layouts,
		const std::vector<std::shared_ptr<UniformBuffer>>& uniformBuffers)
	{
		LOGICAL_DEVICE_LOCATOR

		const auto descriptorPoolPtr = DescriptorPoolLocator::GetDescriptorPool().lock();
		S_ASSERT(!descriptorPoolPtr, "Descriptor Pool reference is null.\n")
		const auto descriptorPool = descriptorPoolPtr->GetDescriptorPool();

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
		allocInfo.pSetLayouts = layouts.data();

		descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);

		VK_CHECK(vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()), "Failed to create descriptor sets.")

		for (size_t i = 0; i < 2; ++i)
		{
			const auto& uniformBuffer = uniformBuffers[i]->GetBuffer();

			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = uniformBuffer;
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = descriptorSets[i];
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &bufferInfo;
			descriptorWrite.pImageInfo = nullptr;		// Optional
			descriptorWrite.pTexelBufferView = nullptr;	// Optional

			vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
		}
	}
}
