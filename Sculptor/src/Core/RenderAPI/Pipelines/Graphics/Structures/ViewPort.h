#pragma once

namespace Sculptor
{
	struct Viewport
	{
		VkViewport vkViewPort{};

		Viewport(const VkExtent2D& swapChainExtent)
		{
			vkViewPort.x = 0.0f;
			vkViewPort.y = 0.0f;
			vkViewPort.width = static_cast<float>(swapChainExtent.width);
			vkViewPort.height = static_cast<float>(swapChainExtent.height);
			vkViewPort.minDepth = 0.0f;
			vkViewPort.maxDepth = 0.0f;
		}
	};
}