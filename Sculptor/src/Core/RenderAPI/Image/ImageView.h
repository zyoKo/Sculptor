#pragma once
#include "Utilities/Macros.h"

namespace Sculptor::Core
{
	class LogicalDevice;

	class ImageView
	{
	protected:
		ImageView() = default;

		virtual ~ImageView() = default;

		virtual void Destroy() const = 0;

		static VkImageView CreateImageView(std::weak_ptr<LogicalDevice> logicalDevice, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT);

	private:
		FRIEND(VulkanTexture)
	};
}
