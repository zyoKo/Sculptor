#pragma once

#include "DescriptorSetLayout.h"
#include "Core/Locators/DescriptorPoolLocator.h"
#include "Core/Locators/LogicalDeviceLocator.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Core/RenderAPI/Buffers/UniformBuffer.h"
#include "Core/RenderAPI/Buffers/Structures/UniformBufferObject.h"

namespace Sculptor::Core
{
	// Don't need to explicitly clean Descriptor Sets, freed along with Descriptor Pool
	class DescriptorSets
	{
	public:
		DescriptorSets() = default;

		~DescriptorSets() = default;

		void Allocate(const std::vector<std::weak_ptr<DescriptorSetLayout>>& descriptorSetLayouts)
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

			const auto descriptorPoolPtr = DescriptorPoolLocator::GetDescriptorPool().lock();
			S_ASSERT(!descriptorPoolPtr, "Descriptor Pool reference is null.\n")
			const auto descriptorPool = descriptorPoolPtr->GetDescriptorPool();

			VkDescriptorSetAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = descriptorPool;
			allocInfo.descriptorSetCount = static_cast<uint32_t>(2); // TODO: Replace 2 with MAX_FRAMES_IN_FLIGHT
			allocInfo.pSetLayouts = layouts.data();

			VK_CHECK(vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()), "Failed to create descriptor sets.")
		}

		void Allocate(const std::weak_ptr<DescriptorSetLayout>& weakDescriptorSetLayout, 
			const std::vector<std::shared_ptr<UniformBuffer>>& uniformBuffers,
			const std::weak_ptr<DescriptorSets>& weakDescriptorSets)
		{
			LOGICAL_DEVICE_LOCATOR

			const auto descriptorSetLayoutPtr = weakDescriptorSetLayout.lock();
			S_ASSERT(!descriptorSetLayoutPtr, "Descriptor Set Layout Ptr is null.")
			const auto descriptorSetLayout = descriptorSetLayoutPtr->GetDescriptorSetLayout();

			const std::vector<VkDescriptorSetLayout> layouts(/* MAX_FRAMES_IN_FLIGHT */ 2, descriptorSetLayout);  // TODO: Replace 2 with MAX_FRAMES_IN_FLIGHT

			const auto descriptorPoolPtr = DescriptorPoolLocator::GetDescriptorPool().lock();
			S_ASSERT(!descriptorPoolPtr, "Descriptor Pool reference is null.\n")
			const auto descriptorPool = descriptorPoolPtr->GetDescriptorPool();

			VkDescriptorSetAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = descriptorPool;
			allocInfo.descriptorSetCount = static_cast<uint32_t>(2); // TODO: Replace 2 with MAX_FRAMES_IN_FLIGHT
			allocInfo.pSetLayouts = layouts.data();

			descriptorSets.resize(2); // TODO: Replace 2 with MAX_FRAMES_IN_FLIGHT
			VK_CHECK(vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()), "Failed to create descriptor sets.")

			for (size_t i = 0; i < 2; ++i)
			{
				const auto& uniformBuffer = uniformBuffers[i]->GetBuffer();

				VkDescriptorBufferInfo bufferInfo{};
				bufferInfo.buffer = uniformBuffer;
				bufferInfo.offset = 0;
				bufferInfo.range = sizeof(UniformBufferObject);

				const auto descriptorSetPtr = weakDescriptorSets.lock();
				S_ASSERT(!descriptorSetPtr, "Descriptor sets are null.")
				const auto& descriptorSet = descriptorSetPtr->GetDescriptorSets()[i];

				VkWriteDescriptorSet descriptorWrite{};
				descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descriptorWrite.dstSet = descriptorSet;
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

		const std::vector<VkDescriptorSet>& GetDescriptorSets() const
		{
			return descriptorSets;
		}

	private:
		std::vector<VkDescriptorSet> descriptorSets;
	};
}
