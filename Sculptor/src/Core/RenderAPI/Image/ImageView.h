#pragma once

namespace Sculptor::Core
{
	class ImageView
	{
	protected:
		ImageView() = default;

		virtual ~ImageView() = default;

		virtual void Destroy() const = 0;

		static VkImageView CreateImageView(VkImage image, VkFormat format);
	};
}