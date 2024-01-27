#include <SculptorPch.h>

#include "LogicalDeviceLocator.h"

#include "Core/RenderAPI/Devices/LogicalDevice.h"

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
