#include <SculptorPch.h>

#include <utility>

#include "ResourceBuilder.h"

#include "Core/Data/Constants.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Core/Locators/LogicalDeviceLocator.h"
#include "Core/RenderAPI/Utility/CreateInfo.h"

namespace Sculptor::Core
{
	ResourceBuilder::ResourceBuilder(ResourceBuilder&& resourceBuilder) noexcept
		:	resourceName(std::move(resourceBuilder.resourceName)),
			descriptorPool(std::exchange(resourceBuilder.descriptorPool, VK_NULL_HANDLE)),
			descriptorSetLayout(std::exchange(resourceBuilder.descriptorSetLayout, VK_NULL_HANDLE)),
			descriptorSets(std::move(resourceBuilder.descriptorSets)),
			descriptorWrites(std::move(resourceBuilder.descriptorWrites)),
			layoutBindings(std::move(resourceBuilder.layoutBindings))
	{ }

	ResourceBuilder::ResourceBuilder(const std::weak_ptr<LogicalDevice>& logicalDevice, std::string&& name,
	                                 std::vector<VkDescriptorSetLayoutBinding>&& descriptorLayoutBindings,
	                                 std::vector<VkWriteDescriptorSet>&& writeDescriptorSets)
	    :	logicalDevice(logicalDevice),
			resourceName(std::move(name)),
			descriptorPool(VK_NULL_HANDLE),
			descriptorSetLayout(VK_NULL_HANDLE),
			descriptorWrites(std::move(writeDescriptorSets)),
			layoutBindings(std::move(descriptorLayoutBindings))
	{
		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto& device = logicalDevicePtr->Get();

		ValidateLayoutBindingsAndWriteDescriptors();

		/*!
		 * Brief: Following are the steps to create descriptor Pool.
		 * Step-1. Create PoolSizes and store descriptor types.
		 * Step-2. Create PoolInfo to store Pool Size data.
		 * Step-3. Allocate Data for Descriptor Pool.
		*/

		// STEP-1: Create Descriptor Pool ///
		std::vector<VkDescriptorPoolSize> poolSize;
		poolSize.reserve(layoutBindings.size());
		for (const auto& layoutBinding : layoutBindings)
		{
			poolSize.emplace_back(layoutBinding.descriptorType, MAX_FRAMES_IN_FLIGHT);
		}

		// STEP-2: Create Descriptor Pool ///
		const auto poolInfo = CreateInfo<VkDescriptorPoolCreateInfo>({
			.maxSets = MAX_FRAMES_IN_FLIGHT,
			.poolSizeCount = static_cast<U32>(poolSize.size()),
			.pPoolSizes = poolSize.data()
		});

		// STEP-3: Create Descriptor Pool ///
		VK_CHECK(vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool), "Failed to create descriptor Pool.")

		const auto descriptorSetLayoutCreateInfo = CreateInfo<VkDescriptorSetLayoutCreateInfo>({
			.bindingCount = static_cast<U32>(layoutBindings.size()),
			.pBindings = layoutBindings.data()
		});

		VK_CHECK(vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCreateInfo, nullptr, &descriptorSetLayout), "Failed to create Descriptor Set Layout!")

		const std::vector<VkDescriptorSetLayout> descriptorSetLayouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);

		const auto descriptorSetAllocateInfo = CreateInfo<VkDescriptorSetAllocateInfo>({
			.descriptorPool = descriptorPool,
			.descriptorSetCount = static_cast<U32>(descriptorSetLayouts.size()),
			.pSetLayouts = descriptorSetLayouts.data()
		});

		descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);

		VK_CHECK(vkAllocateDescriptorSets(device, &descriptorSetAllocateInfo, descriptorSets.data()), 
			"Failed to create descriptor sets.")

		for(size_t i = 0; i < descriptorWrites.size(); ++i)
		{
			descriptorWrites[i].dstBinding = static_cast<U32>(i);
			descriptorWrites[i].dstSet = descriptorSets[i];
		}

		for (size_t i = 0; i < descriptorWrites.size(); ++i)
		{
			vkUpdateDescriptorSets(device, static_cast<U32>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}
	}

	VkDescriptorSetLayout& ResourceBuilder::GetDescriptorSetLayout()
	{
		return descriptorSetLayout;
	}

	const std::vector<VkDescriptorSet>& ResourceBuilder::GetDescriptorSets() const
	{
		return descriptorSets;
	}

	ResourceBuilder::~ResourceBuilder()
	{
		LOGICAL_DEVICE_LOCATOR

		//GetShared<LogicalDevice> logicalDevicePtr { logicalDevice };
		//const auto& device = logicalDevicePtr->Get();

		vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

		vkDestroyDescriptorPool(device, descriptorPool, nullptr);
	}

	void ResourceBuilder::ValidateLayoutBindingsAndWriteDescriptors() const
	{
		for (std::size_t i = 0; i < layoutBindings.size(); ++i) 
		{
			S_ASSERT(layoutBindings[i].descriptorType != descriptorWrites[i].descriptorType, "VkDescriptorType mismatch in descriptor set layout binding and write descriptor set!")
		}
	}
}
