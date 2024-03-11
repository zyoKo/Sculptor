#include <SculptorPch.h>

#include "Texture2D.h"

namespace Sculptor::Core
{
	void Texture2D::Create(const std::string& filePath)
	{ }

	void Texture2D::InitializeTexture(const VkDevice device, const VkPhysicalDevice physicalDevice, int textureWidth,
		int textureHeight, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties)
	{ }

	const VkImage& Texture2D::GetTextureImage() const
	{
		return textureImage;
	}

	const VkDeviceMemory& Texture2D::GetImageMemory() const
	{
		return imageMemory;
	}

	void Texture2D::BindMemory(const VkDevice& device, VkDeviceSize memoryOffset) const
	{
		vkBindImageMemory(device, textureImage, imageMemory, memoryOffset);
	}

	void Texture2D::Destroy(const VkDevice& device) const
	{
		vkDestroyImage(device, textureImage, nullptr);

		vkFreeMemory(device, imageMemory, nullptr);
	}

	/*
	 * This is a protected function
	 */
	Texture2D::Texture2D()
		:	textureImage(VK_NULL_HANDLE),
			imageMemory(VK_NULL_HANDLE)
	{ }
}
