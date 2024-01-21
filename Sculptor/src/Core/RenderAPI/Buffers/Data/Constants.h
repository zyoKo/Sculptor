#pragma once

#include "Core/RenderAPI/Buffers/Structures/Vertex.h"

namespace Sculptor::Core
{
	const std::vector<Vertex> VERTICES = {
		{{ 0.0f, -0.5f }, { 1.0f, 1.0f, 1.0f }},
		{{ 0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f }},
		{{-0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f }}
	};
}
