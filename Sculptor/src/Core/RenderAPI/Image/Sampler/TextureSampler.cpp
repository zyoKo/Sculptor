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
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;

		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto& device = logicalDevicePtr->Get();

		const auto weakPhysicalDevice = logicalDevicePtr->GetPhysicalDevice();
		GetShared<PhysicalDevice> physicalDevicePtr{ weakPhysicalDevice };
		const auto& physicalDevice = physicalDevicePtr->GetPrimaryDevice();

		VkPhysicalDeviceProperties physicalDeviceProperties{};
		vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

		// Do above steps to get maxAnisotropy info based on physical Device limits
		samplerInfo.maxAnisotropy = physicalDeviceProperties.limits.maxSamplerAnisotropy;
		
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE; // VK_FALSE: [0, 1) VK_TRUE: [0, texelWidth) and [0, texelHeight)
		samplerInfo.compareEnable = VK_FALSE;

		// If a comparison function is enabled, then texels will first be compared to a value,
		// and the result of that comparison is used in filtering operations.
		// This is mainly used for percentage-closer filtering on shadow maps. (Project 2: ???)
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;

		VkFormatProperties formatProperties{};
		vkGetPhysicalDeviceFormatProperties(physicalDevice, VK_FORMAT_R8G8B8A8_SRGB, &formatProperties);

		VkImageFormatProperties imageFormatProperties{};
		vkGetPhysicalDeviceImageFormatProperties(
			physicalDevice, 
			VK_FORMAT_R8G8B8A8_SRGB, 
			VK_IMAGE_TYPE_2D, 
			VK_IMAGE_TILING_LINEAR,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			0,
			&imageFormatProperties);

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
