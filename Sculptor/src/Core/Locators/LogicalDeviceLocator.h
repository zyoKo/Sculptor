#pragma once

namespace Sculptor::Core
{
	class LogicalDevice;
}

namespace Sculptor::Core
{
	class LogicalDeviceLocator
	{
	public:
		static void Provide(const std::weak_ptr<LogicalDevice>& device);

		static std::weak_ptr<LogicalDevice> GetLogicalDevice();

	private:
		inline static std::weak_ptr<LogicalDevice> logicalDevice{};
	};
}