#pragma once

#include "Core/RenderAPI/Devices/LogicalDevice.h"

namespace Sculptor::Core
{
	class SupportUtility
	{
	public:
		static VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
		{
			GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
			GetShared<PhysicalDevice> physicalDevicePtr{ logicalDevicePtr->GetPhysicalDevice() };
			const auto physicalDevice = physicalDevicePtr->GetPrimaryDevice();

			for (const auto format : candidates)
			{
				VkFormatProperties formatProperties;
				vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProperties);

				if (tiling == VK_IMAGE_TILING_LINEAR && (formatProperties.linearTilingFeatures & features) == features)
				{
					return format;
				}
				else if (tiling == VK_IMAGE_TILING_OPTIMAL && (formatProperties.optimalTilingFeatures & features) == features)
				{
					return format;
				}
			}

			S_ASSERT(true, "Failed to find supported format!")
			return {};
		}

		static VkFormat FindDepthFormat()
		{
			return FindSupportedFormat(
				{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
				VK_IMAGE_TILING_OPTIMAL,
				VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
		}

		static bool HasStencilComponent(VkFormat format)
		{
			return (format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT);
		}

		static void SetLogicalDevice(std::weak_ptr<LogicalDevice> device) noexcept
		{
			logicalDevice = std::move(device);
		}

	private:
		inline static std::weak_ptr<LogicalDevice> logicalDevice;
	};
}
