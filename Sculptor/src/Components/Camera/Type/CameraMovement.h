#pragma once

namespace Sculptor::Component
{
	enum class CameraMovement
	{
		IDLE,

		FORWARD,

		BACKWARD,

		LEFT,

		RIGHT,

		UP,

		DOWN,

		ROTATE_LEFT,

		ROTATE_RIGHT,

		ZOOM_IN,

		ZOOM_OUT,

		LOOK_UP,

		LOOK_DOWN
	};
}