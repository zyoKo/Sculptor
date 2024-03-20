#include <SculptorPch.h>

#include "Input.h"

namespace Sculptor::Core
{
	std::unique_ptr<Input> Input::instance{ nullptr };

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

	void Input::Update()
	{
		UpdateKeyActionsList();
	}

	Input::Input()
	{
		keyboardActionList.fill(InputAction::NONE);
	}

	void Input::KeyCallback(int key, int scanCode, int action, int mods)
	{
		const KeyCode engineKeyCode = GlfwKeyToEngineKeyCode(key);
		const InputAction engineInputAction = GlfwInputActionToEngineInputAction(action);

		if (engineInputAction != InputAction::REPEAT)
		{
			instance->keyboardActionList[static_cast<int>(engineKeyCode)] = engineInputAction;
			instance->isKeyActionDirty = true;
		}
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
}
