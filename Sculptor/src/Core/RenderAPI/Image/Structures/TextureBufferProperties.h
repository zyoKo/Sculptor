#pragma once

#include "Core/RenderAPI/Buffers/Structures/BufferProperties.h"
#include "Core/RenderAPI/Image/Data/Constants.h"

namespace Sculptor::Core
{
	struct TextureBufferProperties
	{
		std::string texturePath { DEFAULT_TEXTURE_PATH };

		VkDeviceSize imageSize;

		VkBufferUsageFlags usageFlags;

		VkMemoryPropertyFlags propertyFlags;

		explicit operator BufferProperties() const
		{
			return { imageSize, usageFlags, propertyFlags };
		}
	};
}
