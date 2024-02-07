#include <SculptorPch.h>

#include "TextureImageView.h"

#include "Utilities/Macros.h"
#include "Core/Locators/LogicalDeviceLocator.h"

namespace Sculptor::Core
{
	VkImageView TextureImageView::Create(const VkImage image, VkFormat format)
	{
		LOGICAL_DEVICE_LOCATOR

		const VkImageViewCreateInfo viewInfo{
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			nullptr,
			0,
			image,
			VK_IMAGE_VIEW_TYPE_2D,
			format,
			{},
			{
				VK_IMAGE_ASPECT_COLOR_BIT,
				0,
				1,
				0,
				1
			}
		};

		VK_CHECK(vkCreateImageView(device, &viewInfo, nullptr, &imageView), "Failed to create texture image view.")

		return imageView;
	}

	VkImageView TextureImageView::GetImageView() const
	{
		return imageView;
	}
}


