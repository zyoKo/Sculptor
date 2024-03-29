#include <SculptorPch.h>

#include "TextureImageView.h"

#include "Core/RenderAPI/Devices/LogicalDevice.h"

namespace Sculptor::Core
{
	TextureImageView::TextureImageView(std::weak_ptr<LogicalDevice> device) noexcept
		:	logicalDevice(std::move(device)),
			textureImageView(nullptr)
	{ }

	void TextureImageView::Create(const VkImage image, VkFormat format /* = VK_FORMAT_R8G8B8A8_SRGB */)
	{
		textureImageView = CreateImageView(logicalDevice, image, format);
	}

	VkImageView TextureImageView::GetImageView() const
	{
		return textureImageView;
	}

	void TextureImageView::Destroy() const
	{
		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto& device = logicalDevicePtr->Get();

		vkDestroyImageView(device, textureImageView, nullptr);
	}
}


