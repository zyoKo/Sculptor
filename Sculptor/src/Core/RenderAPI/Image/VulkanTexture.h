#pragma once

#include "Texture2D.h"

namespace Sculptor::Core
{
	class VulkanTexture : public Texture2D
	{
	public:
		VulkanTexture() = default;

		void CreateTexture(const VkDevice device, const VkPhysicalDevice physicalDevice, int textureWidth, int textureHeight, VkFormat format, 
			VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties) override;

		void AllocateBuffer(TextureBufferProperties& textureBufferProperties) override;

		void CopyBufferToImage(const VkBuffer buffer, uint32_t width, uint32_t height) const;

	private:
		void TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout) const;
	};
}
