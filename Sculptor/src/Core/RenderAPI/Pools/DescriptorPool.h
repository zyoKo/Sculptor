#pragma once

#include "Core/Core.h"
#include "Utilities/Macros.h"
#include "Core/Locators/LogicalDeviceLocator.h"
#include "Core/RenderAPI/Data/Constants.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"

namespace Sculptor::Core
{
	class DescriptorPool
	{
	public:
		DescriptorPool()
			:	descriptorPool(VK_NULL_HANDLE)
		{ }

		~DescriptorPool() = default;

		void Create(uint32_t descriptorCount = MAX_FRAMES_IN_FLIGHT)
		{
			LOGICAL_DEVICE_LOCATOR

			/***
			 * NOTE: Inadequate descriptor pools are a good example of a problem that the validation layers will not catch:
			 * As of Vulkan 1.1, vkAllocateDescriptorSets may fail with the error code "VK_ERROR_POOL_OUT_OF_MEMORY" if the
			 * pool is not sufficiently large, but the driver may also try to solve the problem internally. This means that
			 * sometimes (depending on hardware, pool size and allocation size) the driver will let us get away with an
			 * allocation that exceeds the limits of our descriptor pool. Other times, vkAllocateDescriptorSets will fail
			 * and return VK_ERROR_POOL_OUT_OF_MEMORY. This can be particularly frustrating if the allocation succeeds on
			 * some machines, but fails on others.
			 *
			 * Since Vulkan shifts the responsiblity for the allocation to the driver, it is no longer a strict requirement
			 * to only allocate as many descriptors of a certain type (VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, etc.) as
			 * specified by the corresponding descriptorCount members for the creation of the descriptor pool. However, it
			 * remains best practise to do so, and in the future, VK_LAYER_KHRONOS_validation will warn about this type of
			 * problem if you enable Best Practice Validation.
			 *
			 * LINK: https://vulkan.org/user/pages/09.events/vulkanised-2023/vulkanised_2023_using_vulkan_validation_effectively.pdf
			 *
			 * TODO: Implement Best Practice Validation Later down the line :D
			***/
			std::array<VkDescriptorPoolSize, 2> poolSizes{};
			poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			poolSizes[0].descriptorCount = descriptorCount;
			poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			poolSizes[1].descriptorCount = descriptorCount;

			const VkDescriptorPoolCreateInfo poolInfo{
				.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.maxSets = descriptorCount,
				.poolSizeCount = static_cast<U32>(poolSizes.size()),
				.pPoolSizes = poolSizes.data()
			};

			VK_CHECK(vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool), "Failed to create descriptor pool.")
		}

		void CleanUp() const
		{
			LOGICAL_DEVICE_LOCATOR

			vkDestroyDescriptorPool(device, descriptorPool, nullptr);
		}

		const VkDescriptorPool& GetDescriptorPool() const
		{
			return descriptorPool;
		}

	private:
		VkDescriptorPool descriptorPool;
	};
}
