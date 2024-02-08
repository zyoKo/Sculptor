#pragma once
#include "Core/RenderAPI/SwapChains/SwapChain.h"

namespace Sculptor::Core
{
	class SwapChainLocator
	{
	public:
		static void Provide(const std::weak_ptr<SwapChain>& swapChain)
		{
			SwapChainLocator::swapChain = swapChain;
		}

		static std::weak_ptr<SwapChain> GetSwapChain()
		{
			return swapChain;
		}

	private:
		inline static std::weak_ptr<SwapChain> swapChain;
	};
}
