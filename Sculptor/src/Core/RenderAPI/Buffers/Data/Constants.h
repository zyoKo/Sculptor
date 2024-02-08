#pragma once

#include "Core/RenderAPI/Buffers/Structures/Vertex.h"

namespace Sculptor::Core
{
	const std::vector<Vertex> VERTICES = {
		{{-0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f}},
		{{ 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f}},
		{{ 0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f}},
		{{-0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f}}
	};

	const std::vector<uint16_t> INDICES = {
		0, 1, 2, 2, 3, 0
	};
}
