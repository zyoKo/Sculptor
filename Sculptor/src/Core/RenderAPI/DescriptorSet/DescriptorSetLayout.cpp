#include <SculptorPch.h>

#include "DescriptorSetLayout.h"

#include "Core/Core.h"
#include "Utilities/Macros.h"
#include "Core/Locators/LogicalDeviceLocator.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"

namespace Sculptor::Core
{
	DescriptorSetLayout::DescriptorSetLayout()
		:	descriptorSetLayout(VK_NULL_HANDLE)
	{ }

	void DescriptorSetLayout::Create()
	{
		constexpr VkDescriptorSetLayoutBinding uboLayoutBinding{
			.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount =  1,
			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,	// which shader stage UBO is in
			.pImmutableSamplers = nullptr	// Optional: Used for image sampling related descriptors
		};

		constexpr VkDescriptorSetLayoutBinding samplerLayoutBinding{
			.binding = 1,
			.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.descriptorCount = 1,
			.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
			.pImmutableSamplers = nullptr
		};

		constexpr std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

		const VkDescriptorSetLayoutCreateInfo layoutInfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.bindingCount = static_cast<U32>(bindings.size()),
			.pBindings = bindings.data()
		};

		LOGICAL_DEVICE_LOCATOR

		VK_CHECK(vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout), "Failed to create descriptor set layout")
	}

	void DescriptorSetLayout::CleanUp() const
	{
		LOGICAL_DEVICE_LOCATOR

		vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
	}

	VkDescriptorSetLayout DescriptorSetLayout::GetDescriptorSetLayout() const
	{
		return descriptorSetLayout;
	}

	const VkDescriptorSetLayout* DescriptorSetLayout::GetDescriptorSetLayoutPointer() const
	{
		return &descriptorSetLayout;
	}
}
