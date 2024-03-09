#pragma once

namespace Sculptor::Core
{
	class LogicalDevice;

	class ImageView
	{
	protected:
		ImageView() = default;

		virtual ~ImageView() = default;

		virtual void Destroy() const = 0;

		static VkImageView CreateImageView(std::weak_ptr<LogicalDevice> logicalDevice, VkImage image, VkFormat format);
	};
}
