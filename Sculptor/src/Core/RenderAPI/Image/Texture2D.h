#pragma once

namespace Sculptor::Core
{
	class Texture2D
	{
	public:
		virtual ~Texture2D() = default;

		virtual void Create(const std::string& filePath);

		virtual void InitializeTexture(const VkDevice device, const VkPhysicalDevice physicalDevice, U32 textureWidth, U32 textureHeight, VkFormat format, 
		                               VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);

		const VkImage& GetTextureImage() const;

		const VkDeviceMemory& GetImageMemory() const;

		void BindMemory(const VkDevice& device, VkDeviceSize memoryOffset) const;

		void Destroy(const VkDevice& device) const;

	protected:
		VkImage textureImage;

		VkDeviceMemory imageMemory;

		Texture2D();
	};
}
