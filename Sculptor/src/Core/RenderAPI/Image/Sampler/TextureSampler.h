#pragma once

namespace Sculptor::Core
{
	class LogicalDevice;
}

namespace Sculptor::Core
{
	class TextureSampler
	{
	public:
		static VkSampler Create(std::weak_ptr<LogicalDevice> logicalDevice);

		static void Destroy(std::weak_ptr<LogicalDevice> logicalDevice, VkSampler textureSampler);
	};
}
