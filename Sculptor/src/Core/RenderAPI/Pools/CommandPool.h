#pragma once

namespace Sculptor::Core
{
	class LogicalDevice;
}

namespace Sculptor::Core
{
	class CommandPool
	{
	public:
		CommandPool() = default;

		CommandPool(const std::weak_ptr<LogicalDevice>& logicalDevice);

		~CommandPool() = default;

		void CreateCommandPool();

		void CleanUp() const;

	private:
		VkCommandPool commandPool{};

		std::weak_ptr<LogicalDevice> logicalDevice;

		friend class CommandBuffer;
	};
}