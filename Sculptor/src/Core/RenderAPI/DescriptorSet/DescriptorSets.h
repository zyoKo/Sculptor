#pragma once

#include "DescriptorSetLayout.h"

namespace Sculptor::Core
{
	class TextureSampler;
	class TextureImageView;
	class UniformBuffer;
	class DescriptorSetLayout;
}

namespace Sculptor::Core
{
	// Don't need to explicitly clean Descriptor Sets, freed along with Descriptor Pool
	class DescriptorSets
	{
	public:
		DescriptorSets() = default;

		~DescriptorSets() = default;

		void Allocate(const std::vector<std::weak_ptr<DescriptorSetLayout>>& descriptorSetLayouts,
			const std::vector<std::shared_ptr<UniformBuffer>>& uniformBuffers,
			const std::tuple<VkImageView, VkSampler>& textureDataList);

		void Allocate(const std::weak_ptr<DescriptorSetLayout>& weakDescriptorSetLayout, 
			const std::vector<std::shared_ptr<UniformBuffer>>& uniformBuffers,
			const std::tuple<VkImageView, VkSampler>& textureDataList);

		const std::vector<VkDescriptorSet>& GetDescriptorSets() const;

	private:
		std::vector<VkDescriptorSet> descriptorSets;

		void AllocateAndUpdateDescriptorSets(const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts,
			const std::vector<std::shared_ptr<UniformBuffer>>& uniformBuffers,
			const std::tuple<VkImageView, VkSampler>& textureDataList);
	};
}
