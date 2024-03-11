#pragma once

#include "Texture2D.h"
#include "Structures/TextureBufferProperties.h"
#include "Utilities/Macros.h"

namespace Sculptor::Core
{
	class LogicalDevice;
	class CommandPool;
}

namespace Sculptor::Core
{
	class VulkanTexture : public Texture2D
	{
	public:
		VulkanTexture() = default;

		VulkanTexture(std::weak_ptr<LogicalDevice> device, std::weak_ptr<CommandPool> cmdPool) noexcept;

		VulkanTexture(std::weak_ptr<LogicalDevice> device, std::weak_ptr<CommandPool> cmdPool, std::string filePath) noexcept;

		void Create(const std::string& filePath) override;

		void SetCommandPool(std::weak_ptr<CommandPool> cmdPool) noexcept;

	protected:
		void InitializeTexture(const VkDevice device, const VkPhysicalDevice physicalDevice, int textureWidth, int textureHeight, VkFormat format, 
			VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties) override;

		void TransitionImageLayout(VkFormat format, VkImageLayout newLayout, VkImageLayout oldLayout = VK_IMAGE_LAYOUT_UNDEFINED) const;

		void CopyBufferToImage(const VkBuffer buffer, uint32_t width, uint32_t height) const;

	private:
		TextureBufferProperties textureBufferProperties{
			.imageSize = 0,	// Calculated after texture data is read from the file
			.usageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			.propertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		LOGICAL_DEVICE

		std::weak_ptr<CommandPool> commandPool;

		std::string fileName;
	};
}
