#include <SculptorPch.h>

#include "LogicalDeviceLocator.h"

namespace Sculptor::Core
{
	void LogicalDeviceLocator::Provide(const std::weak_ptr<LogicalDevice>& device)
	{
		logicalDevice = device;
	}

	std::weak_ptr<LogicalDevice> LogicalDeviceLocator::GetLogicalDevice()
	{
		return logicalDevice;
	}
}
