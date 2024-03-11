#include <SculptorPch.h>

#include "ImageView.h"

#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Core/RenderAPI/Utility/CreateInfo.h"

namespace Sculptor::Core
{
	VkImageView ImageView::CreateImageView(std::weak_ptr<LogicalDevice> logicalDevice, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags /* = VK_IMAGE_ASPECT_COLOR_BIT */)
	{
		GetShared<LogicalDevice> logicalDevicePtr{ std::move(logicalDevice) };
		const auto& device = logicalDevicePtr->Get();

		const auto imageViewInfo = CreateInfo<VkImageViewCreateInfo>({
			.image = image,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = format,
			.subresourceRange = {
				.aspectMask = aspectFlags,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			}
		});

		VkImageView imageView;

		VK_CHECK(vkCreateImageView(device, &imageViewInfo, nullptr, &imageView), "Failed to create texture image view!")

		return imageView;
	}
}
