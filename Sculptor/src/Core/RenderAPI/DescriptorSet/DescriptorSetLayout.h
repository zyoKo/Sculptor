#pragma once

namespace Sculptor::Core
{
	class DescriptorSetLayout
	{
	public:
		DescriptorSetLayout();

		~DescriptorSetLayout() = default;

		void Create();

		void CleanUp() const;

		[[nodiscard]] VkDescriptorSetLayout GetDescriptorSetLayout() const;

		[[nodiscard]] const VkDescriptorSetLayout* GetDescriptorSetLayoutPointer() const;

	private:
		VkDescriptorSetLayout descriptorSetLayout;
	};
}
