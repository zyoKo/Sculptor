#include <SculptorPch.h>

#include "VulkanTexture.h"

#include "Core/Core.h"
#include "Core/RenderAPI/Buffers/Buffer.h"
#include "Utilities/Utilities.h"
#include "Core/Locators/LogicalDeviceLocator.h"

namespace Sculptor::Core
{
	void VulkanTexture::CreateTexture(const VkDevice& device, const VkPhysicalDevice& physicalDevice, int textureWidth, int textureHeight, VkFormat format, 
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

	void VulkanTexture::AllocateBuffer(BufferProperties& bufferProperties)
	{
		LOGICAL_DEVICE_LOCATOR

		int textureWidth{0}, textureHeight{0};

		stbi_uc* texture = Utilities::LoadTexture("./Assets/Textures/texture.png", bufferProperties.bufferSize, &textureWidth, &textureHeight);

		Buffer stagingBuffer{};
		stagingBuffer.Create(bufferProperties);

		void* data;
		vkMapMemory(device, stagingBuffer.GetBufferMemory(), 0, bufferProperties.bufferSize, 0, &data);
		memcpy(data, texture, static_cast<size_t>(bufferProperties.bufferSize));
		vkUnmapMemory(device, stagingBuffer.GetBufferMemory());

		Utilities::DestroyTexture(texture);

		const auto physicalDevicePtr = logicalDevicePtr->GetPhysicalDevice().lock();
		S_ASSERT(physicalDevicePtr == nullptr, "PhysicalDevice is null.")
		const auto& physicalDevice = physicalDevicePtr->GetPrimaryPhysicalDevice();

		CreateTexture(device, physicalDevice, textureWidth, textureHeight, 
			VK_FORMAT_R8G8B8A8_SRGB, 
			VK_IMAGE_TILING_OPTIMAL, 
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	}
}
