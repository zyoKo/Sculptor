#include <SculptorPch.h>

#include "TextureSampler.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"

namespace Sculptor::Core
{
	TextureSampler::TextureSampler(std::weak_ptr<LogicalDevice> logicalDevicePtr) noexcept
		:	logicalDevice(std::move(logicalDevicePtr)),
			textureSampler(nullptr)
	{ }

	void TextureSampler::Create()
	{
		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto& device = logicalDevicePtr->Get();

		const auto weakPhysicalDevice = logicalDevicePtr->GetPhysicalDevice();
		GetShared<PhysicalDevice> physicalDevicePtr{ weakPhysicalDevice };
		const auto& physicalDevice = physicalDevicePtr->GetPrimaryDevice();

		VkPhysicalDeviceProperties physicalDeviceProperties{};
		vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

		const VkSamplerCreateInfo samplerInfo{
			.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,

			.magFilter = VK_FILTER_LINEAR,
			.minFilter = VK_FILTER_LINEAR,
			.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
			.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			.mipLodBias = 0.0f,

			// Do above steps to get maxAnisotropy info based on physical Device limits
			.anisotropyEnable = VK_TRUE,
			.maxAnisotropy = physicalDeviceProperties.limits.maxSamplerAnisotropy,

			// If a comparison function is enabled, then texels will first be compared to a value,
			// and the result of that comparison is used in filtering operations.
			// This is mainly used for percentage-closer filtering on shadow maps. (Project 2: ???)
			.compareEnable = VK_FALSE,
			.compareOp = VK_COMPARE_OP_ALWAYS,

			// [unused] Level of Details Parameters
			.minLod = 0.0f,
			.maxLod = 0.0f,

			.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,

			// VK_FALSE: [0, 1) VK_TRUE: [0, texelWidth) and [0, texelHeight)
			.unnormalizedCoordinates = VK_FALSE, 
		};

		VK_CHECK(vkCreateSampler(device, &samplerInfo, nullptr, &textureSampler), "Failed to create texture sampler.")
	}

	void TextureSampler::Destroy() const
	{
		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto& device = logicalDevicePtr->Get();

		vkDestroySampler(device, textureSampler, nullptr);
	}

	VkSampler TextureSampler::GetTextureSampler() const
	{
		return textureSampler;
	}
}
