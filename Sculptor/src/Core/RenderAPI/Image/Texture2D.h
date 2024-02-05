#pragma once

#include "Core/RenderAPI/Buffers/Structures/BufferProperties.h"
#include "Core/RenderAPI/Interfaces/BufferUtility.h"

namespace Sculptor::Core
{
	class Texture2D : public BufferUtility
	{
	public:
		virtual ~Texture2D() = default;

		virtual void AllocateBuffer(BufferProperties& bufferProperties) = 0;

		virtual void CreateTexture(const VkDevice& device, const VkPhysicalDevice& physicalDevice, int textureWidth, int textureHeight, VkFormat format, 
			VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties) = 0;

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
		VkImage textureImage{};

		VkDeviceMemory imageMemory{};

		Texture2D() = default;
	};
}
