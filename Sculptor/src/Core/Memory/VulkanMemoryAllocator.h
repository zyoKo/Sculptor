#pragma once

#include "Utilities/Macros.h"

namespace Sculptor::Core
{
	class LogicalDevice;
	class VulkanInstanceWrapper;
}

namespace Sculptor::Core
{
	class VulkanMemoryAllocator
	{
	public:
		VulkanMemoryAllocator() = default;

		VulkanMemoryAllocator(const std::weak_ptr<VulkanInstanceWrapper>& instance, const std::weak_ptr<LogicalDevice>& device);

		~VulkanMemoryAllocator() = default;

		void Create();

		const VmaAllocator& Get() const;

		void CleanUp() const;

		VULKAN_INSTANCE_WRAPPER

		LOGICAL_DEVICE

	private:
		VmaAllocator memoryAllocator{};

		friend class ImageAllocator;

		friend class BufferAllocator;
	};

	class ImageAllocator : public VulkanMemoryAllocator
	{
	public:
		ImageAllocator() = default;

		~ImageAllocator() = default;

		void Allocate()
		{
			VkImageCreateInfo imageInfo{};
			imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;

			VmaAllocationCreateInfo allocInfo{};
			allocInfo.usage = VMA_MEMORY_USAGE_AUTO;

			const VkResult result = vmaCreateImage(memoryAllocator, &imageInfo, &allocInfo, &image, &allocation, nullptr);
			// TODO: S_ASSERT
		}

		void Destroy() const;

	private:
		VkImage image{};

		VmaAllocation allocation{};
	};

	class BufferAllocator : public VulkanMemoryAllocator
	{
	public:
		BufferAllocator() = default;

		~BufferAllocator() = default;

		void Allocate()
		{
			VkBufferCreateInfo bufferInfo{};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = static_cast<VkDeviceSize>(65536);
			bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

			VmaAllocationCreateInfo allocInfo{};
			allocInfo.usage = VMA_MEMORY_USAGE_AUTO;

			const VkResult result = vmaCreateBuffer(memoryAllocator, &bufferInfo, &allocInfo, &buffer, &allocation, nullptr);
			// TODO: S_ASSERT
		}

		void DeAllocate() const
		{
			vmaDestroyBuffer(memoryAllocator, buffer, allocation);
		}

	private:
		VkBuffer buffer{};

		VmaAllocation allocation{};
	};
}
