#pragma once

#include "Core/RenderAPI/Buffers/Structures/UniformBufferObject.h"

namespace Sculptor::Core
{
	enum class DescriptorType
	{
		BUFFER,
		IMAGE,
		BUFFER_VIEWS
	};

	struct DescriptorData
	{
		DescriptorType type;

		UniformBufferObject data;

		DescriptorData(DescriptorType type, const UniformBufferObject& data)
			:	type(type)
		{
			switch(type)
			{
			case DescriptorType::BUFFER:
				this->data = data;
				break;

			case DescriptorType::IMAGE:
			case DescriptorType::BUFFER_VIEWS:
				// TODO: Add functionality Later
				break;
			}
		}
	};
}
