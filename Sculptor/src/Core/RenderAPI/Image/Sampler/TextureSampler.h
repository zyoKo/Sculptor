#pragma once
#include "Utilities/Macros.h"

namespace Sculptor::Core
{
	class LogicalDevice;
}

namespace Sculptor::Core
{
	class TextureSampler
	{
	public:
		TextureSampler(std::weak_ptr<LogicalDevice> logicalDevicePtr) noexcept;

		void Create();

		void Destroy() const;

		VkSampler GetTextureSampler() const;

		LOGICAL_DEVICE

	private:
		VkSampler textureSampler;
	};
}
