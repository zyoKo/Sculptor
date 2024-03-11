#include <SculptorPch.h>

#include "CommandPool.h"
#include "Core/RenderAPI/Devices/LogicalDevice.h"
#include "Core/RenderAPI/Utility/CreateInfo.h"

namespace Sculptor::Core
{
	CommandPool::CommandPool()
		:	commandPool(VK_NULL_HANDLE)
	{ }

	CommandPool::CommandPool(std::weak_ptr<LogicalDevice> device) noexcept
		:	logicalDevice(std::move(device))
	{ }

	void CommandPool::Create()
	{
		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto device = logicalDevicePtr->Get();

		const auto& queueFamilyIndices = logicalDevicePtr->GetQueueFamilies().GetQueueFamilyIndices();

		// anther flag for command pool is VK_COMMAND_POOL_CREATE_TRANSIENT_BIT
		// This flag hints that command buffers are rerecorded with new commands very often
		// NOTE: It may change memory allocation behaviour
		auto poolInfo = CreateInfo<VkCommandPoolCreateInfo>({
			.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT // allow command buffers to rerecorded individually (without the flag they all have to be reset together
		});	

		if (!queueFamilyIndices.graphicsFamily.has_value())
		{
			std::cerr << "Queue families are not initialized. Failed to create CommandPool." << std::endl;
			return;
		}
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

		VK_CHECK(vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool), "Failed to create command pool.")
	}

	const VkCommandPool& CommandPool::Get() const
	{
		return commandPool;
	}

	void CommandPool::CleanUp() const
	{
		GetShared<LogicalDevice> logicalDevicePtr{ logicalDevice };
		const auto device = logicalDevicePtr->Get();

		vkDestroyCommandPool(device, commandPool, nullptr);
	}
}
