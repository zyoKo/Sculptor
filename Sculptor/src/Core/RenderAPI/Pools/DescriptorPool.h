#pragma once

#include "Core/Core.h"
#include "Utilities/Macros.h"
#include "Core/Locators/LogicalDeviceLocator.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"

namespace Sculptor::Core
{
	class DescriptorPool
	{
	public:
		DescriptorPool() = default;

		~DescriptorPool() = default;

		void Create(uint32_t descriptorCount)
		{
			LOGICAL_DEVICE_LOCATOR

			VkDescriptorPoolSize poolSize{};
			poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			poolSize.descriptorCount = descriptorCount;

			VkDescriptorPoolCreateInfo poolInfo{};
			poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			poolInfo.poolSizeCount = 1;
			poolInfo.pPoolSizes = &poolSize;
			poolInfo.maxSets = descriptorCount;

			VK_CHECK(vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool), "Failed to create descriptor pool.")
		}

		void CleanUp() const
		{
			LOGICAL_DEVICE_LOCATOR

			vkDestroyDescriptorPool(device, descriptorPool, nullptr);

			int i = 0;
		}

		const VkDescriptorPool& GetDescriptorPool() const
		{
			return descriptorPool;
		}

	private:
		VkDescriptorPool descriptorPool{};
	};
}
