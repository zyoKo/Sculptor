#pragma once

namespace Sculptor::Core
{
	struct QueueFamilyIndices
	{
	private:
		U32 graphicsFamily{ std::numeric_limits<U32>::max() };
		
		U32 presetFamily{ std::numeric_limits<U32>::max() };

		friend class QueueFamilies;

	public:
		bool IsComplete() const
		{
			if (GetGraphicsFamily() && GetPresetFamily())
			{
				return true;
			}

			return false;
		}

		std::optional<U32> GetGraphicsFamily() const
		{
			if (graphicsFamily == std::numeric_limits<U32>::max())
			{
				return std::nullopt;
			}

			return graphicsFamily;
		}

		std::optional<U32> GetPresetFamily() const
		{
			if (presetFamily == std::numeric_limits<U32>::max())
			{
				return std::nullopt;
			}

			return presetFamily;
		}
	};
}
