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
		TextureImageView(std::weak_ptr<LogicalDevice> device) noexcept;

		void Create(const VkImage image, VkFormat format = VK_FORMAT_R8G8B8A8_SRGB);

		VkImageView GetImageView() const;

		void Destroy() const override;

		LOGICAL_DEVICE

	private:
		VkImageView textureImageView;
	};
}

