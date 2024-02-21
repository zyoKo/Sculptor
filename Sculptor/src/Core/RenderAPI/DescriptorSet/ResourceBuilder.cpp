#include <SculptorPch.h>

#include <utility>

#include "ResourceBuilder.h"

namespace Sculptor::Core
{
	ResourceBuilder::ResourceBuilder(ResourceBuilder&& resourceBuilder) noexcept
		:	resourceName(std::move(resourceBuilder.resourceName)),
			descriptorPool(std::exchange(resourceBuilder.descriptorPool, VK_NULL_HANDLE)),
			descriptorSetLayout(std::exchange(resourceBuilder.descriptorSetLayout, VK_NULL_HANDLE)),
			descriptorSets(std::move(resourceBuilder.descriptorSets)),
			descriptorWrites(std::exchange(descriptorWrites, nullptr)),
			layoutBindings(std::exchange(resourceBuilder.layoutBindings, nullptr))
	{ }

	ResourceBuilder::ResourceBuilder(std::weak_ptr<LogicalDevice> logicalDevice, std::string&& name,
	                                 std::vector<VkDescriptorSetLayoutBinding>&& descriptorLayoutBindings,
	                                 std::vector<VkWriteDescriptorSet>&& writeDescriptorSets) noexcept
	    :	logicalDevice(std::move(logicalDevice)),
			resourceName(name),
			descriptorPool(VK_NULL_HANDLE),
			descriptorSetLayout(VK_NULL_HANDLE),
			descriptorWrites(std::move(writeDescriptorSets)),
			layoutBindings(std::move(descriptorLayoutBindings))
	{
		ValidateLayoutBindingsAndWriteDescriptors();

		std::vector<VkDescriptorPoolSize> poolSize;
		poolSize.reserve(layoutBindings.size());

		for (const auto& layoutBinding : layoutBindings)
		{
			poolSize.emplace_back(layoutBinding.descriptorType, 1);
		}

		const VkDescriptorPoolCreateInfo poolInfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.maxSets = 1,
			.poolSizeCount = static_cast<U32>(poolSize.size()),
			.pPoolSizes = poolSize.data()
		};
	}

	void ResourceBuilder::ValidateLayoutBindingsAndWriteDescriptors() const
	{
		for (std::size_t i = 0; i < layoutBindings.size(); ++i) 
		{
			S_ASSERT(layoutBindings[i].descriptorType != descriptorWrites[i].descriptorType,
				"VkDescriptorType mismatch in descriptor set layout binding and write descriptor set!")
		}
	}
}
