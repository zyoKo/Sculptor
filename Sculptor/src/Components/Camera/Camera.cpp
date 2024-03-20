#include <SculptorPch.h>

#include "Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Constants/CameraConstants.h"
#include "Core/Input/Input.h"
#include "Core/Utility/Time.h"

namespace Sculptor::Component
{
	Camera Camera::instance;

	Camera* Camera::GetInstance()
	{
		return &instance;
	}

	void Camera::Initialize(std::weak_ptr<Core::WindowsWindow> window) noexcept
	{
		this->window = std::move(window);

		cameraPosition		= CAMERA_DEFAULT_POSITION;
		worldUp				= CAMERA_DEFAULT_WORLD_UP;
		rotateSpeed			= CAMERA_ROTATE_SPEED;
		zoomSpeed			= CAMERA_ZOOM_SPEED;
		yaw					= CAMERA_YAW;
		pitch				= CAMERA_PITCH;
		movementSpeed		= CAMERA_SPEED;
		mouseSensitivity	= CAMERA_SENSITIVITY;
		zoom				= CAMERA_ZOOM;

		initialPosition		= cameraPosition;
		initialWorldUp		= worldUp;
		initialYaw			= yaw;
		initialPitch		= pitch;
		initialZoom			= zoom;

		UpdateCameraVectors();
	}

	void Camera::Update()
	{
		using namespace Sculptor::Core;

		CameraMovement direction{ CameraMovement::IDLE };

		if (Input::IsKeyHeld(KeyCode::KEY_A))
		{
			direction = CameraMovement::LEFT;
		}
		else if (Input::IsKeyHeld(KeyCode::KEY_W))
		{
			direction = CameraMovement::FORWARD;
		}
		else if (Input::IsKeyHeld(KeyCode::KEY_S))
		{
			direction = CameraMovement::BACKWARD;
		}
		else if (Input::IsKeyHeld(KeyCode::KEY_D))
		{
			direction = CameraMovement::RIGHT;
		}

		ProcessKeyboard(direction);
	}

	const glm::vec3& Camera::GetCameraPosition() const
	{
		return cameraPosition;
	}

	void Camera::SetYaw(float yaw)
	{
		this->yaw = yaw;

		UpdateCameraVectors();
	}

	float Camera::GetYaw() const
	{
		return yaw;
	}

	void Camera::SetPitch(float pitch)
	{
		this->pitch = pitch;

		UpdateCameraVectors();
	}

	float Camera::GetPitch() const
	{
		return pitch;
	}

	void Camera::SetMovementSpeed(float speed)
	{
		movementSpeed = speed;
	}

	float Camera::GetMovementSpeed() const
	{
		return movementSpeed;
	}

	void Camera::SetMouseSensitivity(float sensitivity)
	{
		mouseSensitivity = sensitivity;
	}

	float Camera::GetMouseSensitivity() const
	{
		return mouseSensitivity;
	}

	void Camera::SetZoom(float zoom)
	{
		this->zoom = zoom;
	}

	float Camera::GetZoom() const
	{
		return zoom;
	}

	void Camera::SetZoomSpeed(float speed)
	{
		zoomSpeed = speed;
	}

	float Camera::GetZoomSpeed() const
	{
		return zoomSpeed;
	}

	glm::mat4 Camera::GetProjectionMatrix() const
	{
		GetShared<Core::WindowsWindow> windowPtr{ window };
		auto* windowProperties = windowPtr->GetWindowProperties();

		auto projection = glm::perspective(glm::radians(zoom), static_cast<float>(windowProperties->windowWidth) / static_cast<float>(windowProperties->windowHeight), CAMERA_NEAR_CLIPPING_PLANE, CAMERA_FAR_CLIPPING_PLANE);

		projection[1][1] *= -1;

		return projection;
	}

	void Camera::SetCameraPosition(const glm::vec3& position)
	{
		this->cameraPosition = position;

		UpdateCameraVectors();
	}

	glm::mat4 Camera::GetViewMatrix() const
	{
		return glm::lookAt(cameraPosition, cameraPosition + front, up);
	}

	void Camera::ProcessKeyboard(CameraMovement direction)
	{
		const auto deltaTime = Core::Time::GetDeltaTime();
		const float velocity = movementSpeed * deltaTime;

		switch(direction)
		{
		case CameraMovement::IDLE:
		default:
			break;

		case CameraMovement::FORWARD:
			cameraPosition += front * velocity;
			break;

		case CameraMovement::BACKWARD:
		    cameraPosition -= front * velocity;
			break;

		case CameraMovement::LEFT:
		    cameraPosition -= right * velocity;
			break;

		case CameraMovement::RIGHT:
		    cameraPosition += right * velocity;
			break;

		case CameraMovement::UP:
			cameraPosition.y += velocity;
			break;

		case CameraMovement::DOWN:
			cameraPosition.y -= velocity;
			break;

		case CameraMovement::ROTATE_LEFT:
			yaw -= rotateSpeed * deltaTime;
			break;

		case CameraMovement::ROTATE_RIGHT:
			yaw += rotateSpeed * deltaTime;
			break;

		case CameraMovement::ZOOM_IN:
		    zoom -= zoomSpeed * deltaTime;
			break;

		case CameraMovement::ZOOM_OUT:
		    zoom += zoomSpeed * deltaTime;
			break;

		case CameraMovement::LOOK_UP:
			pitch += velocity;
			break;

		case CameraMovement::LOOK_DOWN:
			pitch -= velocity;
			break;
		}

		if (zoom < 1.0f)
		{
		    zoom = 1.0f;
		}
		else if (zoom > 45.0f)
		{
		    zoom = 45.0f;
		}

		UpdateCameraVectors();
	}

	void Camera::Reset()
	{
	    cameraPosition	= initialPosition;
	    worldUp			= initialWorldUp;
	    yaw				= initialYaw;
	    pitch			= initialPitch;
	    zoom			= initialZoom;
	
	    UpdateCameraVectors();
	}

	void Camera::SetWindowsWindow(std::weak_ptr<Core::WindowsWindow> window) noexcept
	{
		this->window = std::move(window);
	}

	void Camera::UpdateCameraVectors()
	{
		glm::vec3 newFront;
		newFront.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
		newFront.y = std::sin(glm::radians(pitch));
		newFront.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));

		front = glm::normalize(newFront);
		right = glm::normalize(glm::cross(front, worldUp));
		up	  = glm::normalize(glm::cross(right, front));
	}
}
