#pragma once

#include "ImageView.h"
#include "Utilities/Macros.h"

namespace Sculptor::Core
{
	class LogicalDevice;
}

namespace Sculptor::Core
{
	class TextureImageView : public ImageView
	{
	public:
		TextureImageView();

		void Create(const VkImage image, VkFormat format);

		VkImageView GetImageView() const;

		void Destroy() const override;

		LOGICAL_DEVICE

	private:
		VkImageView textureImageView;
	};
}

