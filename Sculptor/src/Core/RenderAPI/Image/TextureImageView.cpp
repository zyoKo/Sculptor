#include <SculptorPch.h>

#include "TextureImageView.h"

#include "Core/Locators/LogicalDeviceLocator.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"

namespace Sculptor::Core
{
	TextureImageView::TextureImageView()
		:	textureImageView(nullptr)
	{ }

	void TextureImageView::Create(const VkImage image, VkFormat format)
	{
		textureImageView = CreateImageView(image, format);
	}

	VkImageView TextureImageView::GetImageView() const
	{
		return textureImageView;
	}

	void TextureImageView::Destroy() const
	{
		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };

		vkDestroyImageView(logicalDevicePtr->Get(), textureImageView, nullptr);
	}
}


