#pragma once

#include "Utilities/Macros.h"

namespace Sculptor::Core
{
	class LogicalDevice;
}

namespace Sculptor::Core
{
	class CommandPool
	{
	public:
		CommandPool();

		CommandPool(std::weak_ptr<LogicalDevice> device) noexcept;

		~CommandPool() = default;

		void Create();

		VkCommandPool Get() const;

		void CleanUp() const;

		LOGICAL_DEVICE

	private:
		VkCommandPool commandPool;

		FRIEND(CommandBuffer)
	};
}
