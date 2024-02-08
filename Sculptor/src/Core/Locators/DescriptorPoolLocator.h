#pragma once

#include "Core/RenderAPI/Pools/DescriptorPool.h"

namespace Sculptor::Core
{
	class DescriptorPoolLocator
	{
	public:
		static void Provide(const std::weak_ptr<DescriptorPool>& pool)
		{
			descriptorPool = pool;
		}

		static std::weak_ptr<DescriptorPool> GetDescriptorPool()
		{
			return descriptorPool;
		}

	private:
		inline static std::weak_ptr<DescriptorPool> descriptorPool;
	};
}
