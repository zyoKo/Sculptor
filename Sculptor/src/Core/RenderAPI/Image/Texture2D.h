#pragma once

namespace Sculptor::Core
{
	class Texture2D
	{
	public:
		virtual ~Texture2D() = default;

		virtual void Create(const std::string& filePath)
		{ }

		virtual void InitializeTexture(const VkDevice device, const VkPhysicalDevice physicalDevice, int textureWidth, int textureHeight, VkFormat format, 
			VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties)
		{ }

		const VkImage& GetTextureImage() const
		{
			return textureImage;
		}

		const VkDeviceMemory& GetImageMemory() const
		{
			return imageMemory;
		}

		void BindMemory(const VkDevice& device, VkDeviceSize memoryOffset) const
		{
			vkBindImageMemory(device, textureImage, imageMemory, memoryOffset);
		}

		void Destroy(const VkDevice& device) const
		{
			vkDestroyImage(device, textureImage, nullptr);

			vkFreeMemory(device, imageMemory, nullptr);
		}

	protected:
		VkImage textureImage;

		VkDeviceMemory imageMemory;

		Texture2D()
			:	textureImage(VK_NULL_HANDLE),
				imageMemory(VK_NULL_HANDLE)
		{ }
	};
}
