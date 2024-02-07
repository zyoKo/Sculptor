#include <SculptorPch.h>

#include "VulkanTexture.h"

#include "Core/Core.h"
#include "Core/RenderAPI/Buffers/Buffer.h"
#include "Utilities/Utilities.h"
#include "Core/Locators/LogicalDeviceLocator.h"
#include "Core/RenderAPI/Buffers/CommandBuffer.h"
#include "Core/Locators/CommandPoolLocator.h"

namespace Sculptor::Core
{
	void VulkanTexture::CreateTexture(const VkDevice device, const VkPhysicalDevice physicalDevice, int textureWidth, int textureHeight, VkFormat format, 
		VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties)
	{
		const VkImageCreateInfo imageInfo{
			VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			nullptr,
			0,
			VK_IMAGE_TYPE_2D,
			format,
			{
				static_cast<uint32_t>(textureWidth),
				static_cast<uint32_t>(textureHeight),
				1
			},
			1,
			1,
			VK_SAMPLE_COUNT_1_BIT,
			tiling,
			usage,
			VK_SHARING_MODE_EXCLUSIVE,
			0,
			nullptr,
			VK_IMAGE_LAYOUT_UNDEFINED
		};

		VK_CHECK(vkCreateImage(device, &imageInfo, nullptr, &textureImage), "Failed to create texture.")

		VkMemoryRequirements memRequirements{};
		vkGetImageMemoryRequirements(device, textureImage, &memRequirements);

		const VkMemoryAllocateInfo allocInfo{
			VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			nullptr,
			memRequirements.size,
			FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties)
		};

		VK_CHECK(vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory), "Failed to create image.")

		BindMemory(device, 0);
	}

	void VulkanTexture::AllocateBuffer(TextureBufferProperties& textureBufferProperties)
	{
		LOGICAL_DEVICE_LOCATOR

		int textureWidth{0}, textureHeight{0};

		stbi_uc* texture = Utilities::LoadTexture("./Assets/Textures/texture.jpg", textureBufferProperties.imageSize, &textureWidth, &textureHeight);

		Buffer stagingBuffer{};
		stagingBuffer.Create(static_cast<BufferProperties>(textureBufferProperties));

		void* data;
		vkMapMemory(device, stagingBuffer.GetBufferMemory(), 0, textureBufferProperties.imageSize, 0, &data);
		memcpy(data, texture, static_cast<size_t>(textureBufferProperties.imageSize));
		vkUnmapMemory(device, stagingBuffer.GetBufferMemory());

		Utilities::DestroyTexture(texture);

		const auto physicalDevicePtr = logicalDevicePtr->GetPhysicalDevice().lock();
		S_ASSERT(physicalDevicePtr == nullptr, "PhysicalDevice is null.")
		const auto& physicalDevice = physicalDevicePtr->GetPrimaryPhysicalDevice();

		constexpr VkFormat imageFormat = VK_FORMAT_R8G8B8A8_SRGB;

		CreateTexture(device, physicalDevice, textureWidth, textureHeight, 
			imageFormat, 
			VK_IMAGE_TILING_OPTIMAL, 
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		CopyBufferToImage(stagingBuffer.GetBuffer(), static_cast<uint32_t>(textureWidth), static_cast<uint32_t>(textureHeight));
		TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		stagingBuffer.Destroy();
	}

	void VulkanTexture::CopyBufferToImage(const VkBuffer buffer, uint32_t width, uint32_t height) const
	{
		COMMAND_POOL_LOCATOR

		LOGICAL_DEVICE_LOCATOR

		const VkCommandBuffer commandBuffer = CommandBuffer::BeginSingleTimeCommand(cmdPool, device);

		const VkBufferImageCopy region{
			0,
			0,
			0,
			{
				VK_IMAGE_ASPECT_COLOR_BIT,
				0,
				0,
				1
			},
			{ 0, 0, 0 },
			{ width, height, 1 }
		};

		vkCmdCopyBufferToImage(commandBuffer, buffer, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		CommandBuffer::EndSingleTimeCommand(commandBuffer);
	}

	void VulkanTexture::TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout) const
	{
		COMMAND_POOL_LOCATOR

		LOGICAL_DEVICE_LOCATOR

		const VkCommandBuffer commandBuffer = CommandBuffer::BeginSingleTimeCommand(cmdPool, device);

		VkImageMemoryBarrier barrier{
			VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			nullptr,
			0, // Optional
			0, // Optional
			oldLayout,
			newLayout,
			VK_QUEUE_FAMILY_IGNORED,
			VK_QUEUE_FAMILY_IGNORED,
			textureImage,
			{
				VK_IMAGE_ASPECT_COLOR_BIT,
				0,
				1,
				0,
				1
			}
		};

		VkPipelineStageFlags sourceStage{ VK_PIPELINE_STAGE_NONE };
		VkPipelineStageFlags destinationStage{ VK_PIPELINE_STAGE_NONE };

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else
		{
			S_ASSERT(false, "Unsupported Layout Transition.")
		}

		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		CommandBuffer::EndSingleTimeCommand(commandBuffer);
	}
}
