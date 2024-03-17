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

		// STEP-1: Create Descriptor Pool ///
		std::vector<VkDescriptorPoolSize> poolSize;
		poolSize.reserve(layoutBindings.size());
		for (const auto& layoutBinding : layoutBindings)
		{
			poolSize.emplace_back(layoutBinding.descriptorType, MAX_FRAMES_IN_FLIGHT);
		}

		const auto poolInfo = CreateInfo<VkDescriptorPoolCreateInfo>({
			.maxSets		= MAX_FRAMES_IN_FLIGHT,
			.poolSizeCount	= static_cast<U32>(poolSize.size()),
			.pPoolSizes		= poolSize.data()
		});

		VK_CHECK(vkCreateDescriptorPool(device, &poolInfo, VK_NULL_HANDLE, &descriptorPool), "Failed to create descriptor Pool.")
		/////////////////////////////////////

		// STEP-2: Create Descriptor Pool Layout ///
		const auto descriptorSetLayoutCreateInfo = CreateInfo<VkDescriptorSetLayoutCreateInfo>({
			.bindingCount	= static_cast<U32>(layoutBindings.size()),
			.pBindings		= layoutBindings.data()
		});

		VK_CHECK(vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCreateInfo, VK_NULL_HANDLE, &descriptorSetLayout), "Failed to create Descriptor Set Layout!")
		////////////////////////////////////////////

		// STEP-3: Allocate Descriptor Sets //
		const std::vector<VkDescriptorSetLayout> descriptorSetLayouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);

		const auto descriptorSetAllocateInfo = CreateInfo<VkDescriptorSetAllocateInfo>({
			.descriptorPool		= descriptorPool,
			.descriptorSetCount = static_cast<U32>(descriptorSetLayouts.size()),
			.pSetLayouts		= descriptorSetLayouts.data()
		});

		descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);

		VK_CHECK(vkAllocateDescriptorSets(device, &descriptorSetAllocateInfo, descriptorSets.data()), "Failed to create descriptor sets.")
		//////////////////////////////////////

		for(size_t i = 0, j = 0; i < descriptorWrites.size(); i += 2, ++j)
		{
			descriptorWrites[i].dstBinding		= static_cast<U32>(j);
			descriptorWrites[i + 1].dstBinding  = static_cast<U32>(j);
			descriptorWrites[i].dstSet			= descriptorSets[j];
			descriptorWrites[i + 1].dstSet		= descriptorSets[j];
		}

		for (size_t i = 1, j = descriptorWrites.size() / 2; i < descriptorWrites.size() / 2; ++i, ++j)
		{
			std::swap(descriptorWrites[i], descriptorWrites[j]);
		}

		//for (size_t i = 0; i < descriptorWrites.size() / 2; ++i)
		//{
		//	vkUpdateDescriptorSets(device, static_cast<U32>(descriptorWrites.size() / MAX_FRAMES_IN_FLIGHT), &descriptorWrites[i * 2], 0, VK_NULL_HANDLE);
		//}

		vkUpdateDescriptorSets(device, static_cast<U32>(descriptorWrites.size()), descriptorWrites.data(), 0, VK_NULL_HANDLE);

		//for (size_t i = 0; i < descriptorWrites.size(); ++i)
		//{
		//	vkUpdateDescriptorSets(device, static_cast<U32>(descriptorWrites.size()), descriptorWrites.data(), 0, VK_NULL_HANDLE);
		//}
	}

	VkDescriptorSetLayout ResourceBuilder::GetDescriptorSetLayout() const
	{
		return descriptorSetLayout;
	}

	const std::vector<VkDescriptorSet>& ResourceBuilder::GetDescriptorSets() const
	{
		return descriptorSets;
	}

	void ResourceBuilder::CleanUp() const
	{
		LOGICAL_DEVICE_LOCATOR

		//GetShared<LogicalDevice> logicalDevicePtr { logicalDevice };
		//const auto& device = logicalDevicePtr->Get();

		vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

		vkDestroyDescriptorPool(device, descriptorPool, nullptr);
	}

	void ResourceBuilder::ValidateLayoutBindingsAndWriteDescriptors() const
	{
		for (std::size_t i = 0, j = 0; i < layoutBindings.size(); ++i, j += MAX_FRAMES_IN_FLIGHT) 
		{
			S_ASSERT(layoutBindings[i].descriptorType != descriptorWrites[j].descriptorType, "VkDescriptorType mismatch in descriptor set layout binding and write descriptor set!")
		}
	}
}
