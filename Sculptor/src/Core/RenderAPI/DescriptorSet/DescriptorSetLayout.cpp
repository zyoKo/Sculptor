#include <SculptorPch.h>

#include "DescriptorSetLayout.h"

#include "Core/Core.h"
#include "Utilities/Macros.h"
#include "Core/Locators/LogicalDeviceLocator.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"

namespace Sculptor::Core
{
	void DescriptorSetLayout::Create()
	{
		VkDescriptorSetLayoutBinding uboLayoutBinding{};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;	// which shader stage UBO is in
		uboLayoutBinding.pImmutableSamplers = nullptr;	// Optional: Used for image sampling related descriptors

		VkDescriptorSetLayoutBinding samplerLayoutBinding{};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<U32>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		LOGICAL_DEVICE_LOCATOR

		VK_CHECK(vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout), "Failed to create descriptor set layout")
	}

	void DescriptorSetLayout::CleanUp() const
	{
		LOGICAL_DEVICE_LOCATOR

		vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
	}

	const VkDescriptorSetLayout& DescriptorSetLayout::GetDescriptorSetLayout() const
	{
		return descriptorSetLayout;
	}
}
