#include <SculptorPch.h>

#include "DescriptorSetLayout.h"

#include "Core/Core.h"
#include "Utilities/Macros.h"
#include "Core/Locators/LogicalDeviceLocator.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Core/RenderAPI/Utility/CreateInfo.h"

namespace Sculptor::Core
{
	DescriptorSetLayout::DescriptorSetLayout()
		:	descriptorSetLayout(VK_NULL_HANDLE)
	{ }

	void DescriptorSetLayout::Create()
	{
		LOGICAL_DEVICE_LOCATOR

		constexpr VkDescriptorSetLayoutBinding uboLayoutBinding{
			.binding			= 0,
			.descriptorType		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount	= 1,
			.stageFlags			= VK_SHADER_STAGE_VERTEX_BIT,	// which shader stage UBO is in
			.pImmutableSamplers = nullptr	// Optional: Used for image sampling related descriptors
		};

		constexpr VkDescriptorSetLayoutBinding samplerLayoutBinding{
			.binding			= 1,
			.descriptorType		= VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.descriptorCount	= 1,
			.stageFlags			= VK_SHADER_STAGE_FRAGMENT_BIT,
			.pImmutableSamplers = nullptr
		};

		constexpr std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

		const auto descriptorSetLayoutInfo = CreateInfo<VkDescriptorSetLayoutCreateInfo>({
			.bindingCount	= static_cast<U32>(bindings.size()),
			.pBindings		= bindings.data()
		});

		VK_CHECK(vkCreateDescriptorSetLayout(device, &descriptorSetLayoutInfo, nullptr, &descriptorSetLayout), "Failed to create descriptor set layout")
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
