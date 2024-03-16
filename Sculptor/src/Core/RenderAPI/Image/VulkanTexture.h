#pragma once

#include "Texture2D.h"
#include "Components/Data/Constants.h"
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
		VulkanTexture();

		VulkanTexture(std::weak_ptr<LogicalDevice> device, std::weak_ptr<CommandPool> cmdPool) noexcept;

		VulkanTexture(std::weak_ptr<LogicalDevice> device, std::weak_ptr<CommandPool> cmdPool, std::string filePath) noexcept;

		void Create(const std::string& filePath = std::string(DEFAULT_TEXTURE)) override;

		void SetCommandPool(std::weak_ptr<CommandPool> cmdPool) noexcept;

		VkImageView GetTextureImageView() const;

		virtual void CleanUp() const;

	protected:
		void InitializeTexture(const VkDevice device, const VkPhysicalDevice physicalDevice, U32 textureWidth, U32 textureHeight, VkFormat format, 
							   VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties) override;

		void TransitionImageLayout(VkFormat format, VkImageLayout newLayout, VkImageLayout oldLayout = VK_IMAGE_LAYOUT_UNDEFINED) const;

		void CopyBufferToImage(const VkBuffer buffer, uint32_t width, uint32_t height) const;

		LOGICAL_DEVICE

	private:
		TextureBufferProperties textureBufferProperties{
			.imageSize = 0,	// Calculated after texture data is read from the file
			.usageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			.propertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		std::weak_ptr<CommandPool> commandPool;

		std::string fileName;

		VkImageView textureImageView;

		void CreateTextureImageView(VkFormat format = VK_FORMAT_R8G8B8A8_SRGB);
	};
}
