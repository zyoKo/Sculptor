#include <SculptorPch.h>

#include "Input.h"

namespace Sculptor::Core
{
#pragma region STATIC_VARIABLES_INITIALIZATION

	std::unique_ptr<Input> Input::instance{ nullptr };

	bool Input::isCursorVisible{ true };

	bool Input::isCursorVisibilityDirty{ false };

#pragma endregion

	Input* Input::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = std::unique_ptr<Input>(new Input());
		}

		return instance.get();
	}

	void Input::Initialize()
	{
		GetInstance();
	}

	void Input::Update()
	{
		UpdateKeyActionsList();
	}

	bool Input::IsKeyPressed(KeyCode keyCode)
	{
		return instance->keyboardActionList[static_cast<int>(keyCode)] == InputAction::PRESSED;
	}

	bool Input::IsKeyReleased(KeyCode keyCode)
	{
		return instance->keyboardActionList[static_cast<int>(keyCode)] == InputAction::RELEASED;
	}

	bool Input::IsKeyHeld(KeyCode keyCode)
	{
		return instance->keyboardActionList[static_cast<int>(keyCode)] == InputAction::HELD;
	}

	void Input::SetCursorHidden(bool value)
	{
		instance->isCursorVisibilityDirty = true;

		instance->isCursorVisible = value;
	}

	double Input::GetMouseX()
	{
		return instance->mouseX;
	}

	double Input::GetMouseY()
	{
		return instance->mouseY;
	}

#pragma region PRIVATE_FUNCTIONS

	Input::Input()
		:	isKeyActionDirty{ true },
			mouseX{ 0.0 },
			mouseY{ 0.0 }
	{
		keyboardActionList.fill(InputAction::NONE);
	}

	void Input::KeyCallback(int key, int scanCode, int action, int mods)
	{
		const KeyCode engineKeyCode = GlfwKeyToEngineKeyCode(key);
		const InputAction engineInputAction = GlfwInputActionToEngineInputAction(action);

		// TODO: CHANGE THIS ASSERT TO A WARNING (DON'T CRASH)
		S_ASSERT(static_cast<int>(engineKeyCode) < 0 || static_cast<int>(engineKeyCode) >= static_cast<int>(KeyCode::TOTAL_KEYS), "Invalid Key Used with KeyCode: {0}", static_cast<int>(engineKeyCode));

		if (engineInputAction != InputAction::REPEAT)
		{
			instance->keyboardActionList[static_cast<int>(engineKeyCode)] = engineInputAction;
			instance->isKeyActionDirty = true;
		}
	}

	void Input::MousePositionCallback(double xPosition, double yPosition)
	{
		std::cout << "Mouse Position: (" << xPosition << ", " << yPosition << ")\n";
	}

	void Input::UpdateKeyActionsList()
	{
		if (instance->isKeyActionDirty == true)
		{
			for (auto& keyAction : instance->keyboardActionList)
			{
				if (keyAction == InputAction::PRESSED)
				{
					keyAction = InputAction::HELD;
				}
				else if (keyAction == InputAction::RELEASED)
				{
					keyAction = InputAction::NONE;
				}
			}

			instance->isKeyActionDirty = false;
		}
	}

#pragma endregion
}
