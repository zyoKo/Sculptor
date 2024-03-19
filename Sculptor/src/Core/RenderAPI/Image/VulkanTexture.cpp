#include <SculptorPch.h>

#include "VulkanTexture.h"

#include "ImageView.h"
#include "Core/Core.h"
#include "Core/RenderAPI/Buffers/Buffer.h"
#include "Utilities/Utilities.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Core/RenderAPI/Buffers/CommandBuffer.h"
#include "Core/RenderAPI/Pools/CommandPool.h"
#include "Core/RenderAPI/Utility/CreateInfo.h"
#include "Core/RenderAPI/Utility/SupportUtility.h"
#include "Utilities/BufferUtility.h"
#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	VulkanTexture::VulkanTexture()
		:	textureImageView(VK_NULL_HANDLE)
	{ }

	VulkanTexture::VulkanTexture(std::weak_ptr<LogicalDevice> device, std::weak_ptr<CommandPool> cmdPool) noexcept
		:	logicalDevice(std::move(device)),
			commandPool(std::move(cmdPool)),
			fileName("Default"),
			textureImageView(VK_NULL_HANDLE)
	{ }

	VulkanTexture::VulkanTexture(std::weak_ptr<LogicalDevice> device, std::weak_ptr<CommandPool> cmdPool, std::string filePath) noexcept
		:	logicalDevice(std::move(device)),
			commandPool(std::move(cmdPool)),
			fileName("Default"),
			textureImageView(VK_NULL_HANDLE)
	{
		fileName = Utilities::ExtractFileNameFromFilePath(filePath);

		Create(filePath);
	}

	void VulkanTexture::Create(const std::string& filePath /* = DEFAULT_TEXTURE */)
	{
		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto device = logicalDevicePtr->Get();

		GetShared<PhysicalDevice> physicalDevicePtr { logicalDevicePtr->GetPhysicalDevice() };
		const auto& physicalDevice = physicalDevicePtr->GetPrimaryDevice();

		fileName = Utilities::ExtractFileNameFromFilePath(filePath);

		int textureWidth{}, textureHeight{};

		stbi_uc* texture = Utilities::LoadTexture(filePath, textureBufferProperties.imageSize, &textureWidth, &textureHeight);

		Buffer stagingBuffer{};
		stagingBuffer.Create(static_cast<BufferProperties>(textureBufferProperties));

		void* data;
		vkMapMemory(device, stagingBuffer.GetBufferMemory(), 0, textureBufferProperties.imageSize, 0, &data);
		memcpy(data, texture, static_cast<size_t>(textureBufferProperties.imageSize));
		vkUnmapMemory(device, stagingBuffer.GetBufferMemory());

		Utilities::DestroyTexture(texture);

		InitializeTexture(device, physicalDevice, textureWidth, textureHeight, 
			VK_FORMAT_R8G8B8A8_SRGB, 
			VK_IMAGE_TILING_OPTIMAL, 
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		TransitionImageLayout(VK_FORMAT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		CopyBufferToImage(stagingBuffer.GetBuffer(), static_cast<uint32_t>(textureWidth), static_cast<uint32_t>(textureHeight));
		TransitionImageLayout(VK_FORMAT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		stagingBuffer.Destroy();

		CreateTextureImageView(/* = VK_FORMAT_R8G8B8A8_SRGB */);
	}

	void VulkanTexture::SetCommandPool(std::weak_ptr<CommandPool> cmdPool) noexcept
	{
		commandPool = std::move(cmdPool);
	}

	VkImageView VulkanTexture::GetTextureImageView() const
	{
		return textureImageView;
	}

	void VulkanTexture::CleanUp() const
	{
		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto device = logicalDevicePtr->Get();

		Destroy(device);

		vkDestroyImageView(device, textureImageView, nullptr);
	}

	void VulkanTexture::CopyBufferToImage(const VkBuffer buffer, uint32_t width, uint32_t height) const
	{
		GetShared<CommandPool> commandPoolPtr{ commandPool };
		const auto cmdPool = commandPoolPtr->Get();

		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto device = logicalDevicePtr->Get();

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

	void VulkanTexture::CreateTextureImageView(VkFormat format /* = VK_FORMAT_R8G8B8A8_SRGB */)
	{
		textureImageView = ImageView::CreateImageView(logicalDevice, textureImage, format);
	}

	void VulkanTexture::InitializeTexture(const VkDevice device, const VkPhysicalDevice physicalDevice, U32 textureWidth, U32 textureHeight, VkFormat format, 
	                                      VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties)
	{
		const auto imageInfo = CreateInfo<VkImageCreateInfo>({
			.imageType = VK_IMAGE_TYPE_2D,
			.format = format,
			.extent = {
				textureWidth,
				textureHeight,
				1
			},
			.mipLevels = 1,
			.arrayLayers = 1,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.tiling = tiling,
			.usage = usage,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 0,
			.pQueueFamilyIndices = VK_NULL_HANDLE,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
		});

		VK_CHECK(vkCreateImage(device, &imageInfo, nullptr, &textureImage), "Failed to create texture.")

		VkMemoryRequirements memRequirements{};
		vkGetImageMemoryRequirements(device, textureImage, &memRequirements);

		const auto memoryAllocateInfo = CreateInfo<VkMemoryAllocateInfo>({
			.allocationSize = memRequirements.size,
			.memoryTypeIndex = BufferUtility::FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties)
		});

		VK_CHECK(vkAllocateMemory(device, &memoryAllocateInfo, nullptr, &imageMemory), "Failed to create image.")

		BindMemory(device, 0);
	}

	void VulkanTexture::TransitionImageLayout(VkFormat format, VkImageLayout newLayout, VkImageLayout oldLayout /* = VK_IMAGE_LAYOUT_UNDEFINED */) const
	{
		GetShared<CommandPool> commandPoolPtr{ commandPool };
		const auto cmdPool = commandPoolPtr->Get();

		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto device = logicalDevicePtr->Get();

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

		if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

			if (SupportUtility::HasStencilComponent(format))
			{
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
		}

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
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask =	VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else
		{
			S_ASSERT(true, "Unsupported Layout Transition.")
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
