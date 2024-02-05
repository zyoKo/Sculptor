#pragma once

#include "Texture2D.h"

namespace Sculptor::Core
{
	class VulkanTexture : public Texture2D
	{
	public:
		VulkanTexture() = default;

		void CreateTexture(const VkDevice& device, const VkPhysicalDevice& physicalDevice, int textureWidth, int textureHeight, VkFormat format, 
			VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties) override;

		void AllocateBuffer(BufferProperties& bufferProperties) override;
	};
}
