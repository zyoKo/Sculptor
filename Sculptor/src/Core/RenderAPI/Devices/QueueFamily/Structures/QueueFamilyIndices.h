#pragma once

namespace Sculptor::Core
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;

		bool IsGraphicsFamilyComplete() const { return graphicsFamily.has_value(); }
	};
}
