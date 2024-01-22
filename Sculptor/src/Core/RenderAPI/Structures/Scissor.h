#pragma once

namespace Sculptor
{
	struct Scissor
	{
		VkRect2D scissor{};

		Scissor(const VkExtent2D& swapChainExtent)
		{
			scissor.offset = { 0, 0 };
			scissor.extent = swapChainExtent;
		}
	};
}