#pragma once

#include "Core/RenderAPI/Pools/CommandPool.h"

namespace Sculptor::Core
{
	class CommandPoolLocator
	{
	public:
		static void Provide(const std::weak_ptr<CommandPool>& pool)
		{
			commandPool = pool;
		}

		static std::weak_ptr<CommandPool> GetCommandPool()
		{
			return commandPool;
		}

	private:
		inline static std::weak_ptr<CommandPool> commandPool{};
	};
}
