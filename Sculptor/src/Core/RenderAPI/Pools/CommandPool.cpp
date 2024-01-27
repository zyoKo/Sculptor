#include <SculptorPch.h>

#include "CommandPool.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Utilities/Logger/Assert.h"

namespace Sculptor::Core
{
	CommandPool::CommandPool(const std::weak_ptr<LogicalDevice>& logicalDevice)
		:	logicalDevice(logicalDevice)
	{ }

	void CommandPool::CreateCommandPool()
	{
		const auto logicalDevicePtr = logicalDevice.lock();
		if (!logicalDevicePtr)
		{
			std::cerr << "Logical device reference is null.\nFailed to create CommandPool." << std::endl;
			return;
		}
		const auto& device = logicalDevicePtr->Get();
		const auto& queueFamilyIndices = logicalDevicePtr->GetQueueFamilies().GetQueueFamilyIndices();

		// anther flag for command pool is VK_COMMAND_POOL_CREATE_TRANSIENT_BIT
		// This flag hints that command buffers are rerecorded with new commands very often
		// NOTE: It may change memory allocation behaviour
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;	// allow command buffers to rerecorded individually (without the flag they all have to be reset together

		if (!queueFamilyIndices.graphicsFamily.has_value())
		{
			std::cerr << "Queue families are not initialized. Failed to create CommandPool." << std::endl;
			return;
		}
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

		const auto result = vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool);
		S_ASSERT(result != VK_SUCCESS, "Failed to create command pool.");
	}

	const VkCommandPool& CommandPool::Get() const
	{
		return commandPool;
	}

	void CommandPool::CleanUp() const
	{
		const auto logicalDevicePtr = logicalDevice.lock();
		if (!logicalDevicePtr)
		{
			std::cerr << "Logical device reference is null.\nFailed to create CommandPool." << std::endl;
			return;
		}
		const auto& device = logicalDevicePtr->Get();

		vkDestroyCommandPool(device, commandPool, nullptr);
	}
}
