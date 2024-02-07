#pragma once

namespace Sculptor::Core
{
	class TextureImageView
	{
	public:
		TextureImageView() = default;

		~TextureImageView() = default;

		VkImageView Create(const VkImage image, VkFormat format);

		VkImageView GetImageView() const;

	private:
		VkImageView imageView;
	};
}

