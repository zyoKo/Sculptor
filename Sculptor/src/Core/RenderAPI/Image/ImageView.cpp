#include <SculptorPch.h>

#include "ImageView.h"

#include "Utilities/Macros.h"
#include "Core/Locators/LogicalDeviceLocator.h"

namespace Sculptor::Core
{
	VkImageView ImageView::CreateImageView(VkImage image, VkFormat format)
	{
		LOGICAL_DEVICE_LOCATOR

		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VkImageView imageView;

		VK_CHECK(vkCreateImageView(device, &viewInfo, nullptr, &imageView), "Failed to create texture image view!")

		return imageView;
	}
}
