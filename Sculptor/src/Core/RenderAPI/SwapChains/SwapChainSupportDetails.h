#pragma once

namespace Sculptor::Core
{
	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;

		std::vector<VkSurfaceFormatKHR> formats;

		std::vector<VkPresentModeKHR> presentModes;
	};
}