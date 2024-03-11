#pragma once

#include "Utilities/Macros.h"

namespace Sculptor::Core
{
	class LogicalDevice;
}

namespace Sculptor::Core
{
	class ResourceBuilder
	{
	public:
		ResourceBuilder(ResourceBuilder&& resourceBuilder) noexcept;

		ResourceBuilder(const std::weak_ptr<LogicalDevice>& logicalDevice,
		                std::string&& name,
		                std::vector<VkDescriptorSetLayoutBinding>&& descriptorLayoutBindings,
		                std::vector<VkWriteDescriptorSet>&& writeDescriptorSets);

		ResourceBuilder(ResourceBuilder&) = delete;

		VkDescriptorSetLayout& GetDescriptorSetLayout();

		const std::vector<VkDescriptorSet>& GetDescriptorSets() const;

		~ResourceBuilder();

		LOGICAL_DEVICE

	private:
		std::string resourceName;

		VkDescriptorPool descriptorPool;

		VkDescriptorSetLayout descriptorSetLayout;

		std::vector<VkDescriptorSet> descriptorSets;

		std::vector<VkWriteDescriptorSet> descriptorWrites;

		std::vector<VkDescriptorSetLayoutBinding> layoutBindings;

		void ValidateLayoutBindingsAndWriteDescriptors() const;
	};
}
