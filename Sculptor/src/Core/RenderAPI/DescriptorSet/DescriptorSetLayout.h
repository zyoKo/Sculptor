#pragma once

namespace Sculptor::Core
{
	class DescriptorSetLayout
	{
	public:
		DescriptorSetLayout() = default;

		~DescriptorSetLayout() = default;

		void Create();

		void CleanUp() const;

		const VkDescriptorSetLayout& GetDescriptorSetLayout() const;

	private:
		VkDescriptorSetLayout descriptorSetLayout{};
	};
}
