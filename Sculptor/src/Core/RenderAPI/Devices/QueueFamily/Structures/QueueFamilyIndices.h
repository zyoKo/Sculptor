#pragma once

namespace Sculptor::Core
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;

		std::optional<uint32_t> presetFamily;

		bool IsComplete() const
		{
			return graphicsFamily.has_value() && presetFamily.has_value();
		}
	};
}
