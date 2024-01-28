#pragma once

#include <glm/glm.hpp>

namespace Sculptor::Component
{
	class Mesh
	{
	public:
		Mesh() = default;

		~Mesh() = default;

	private:
		std::vector<glm::vec2> vertices;

		std::vector<glm::vec3> colors;
	};
}
