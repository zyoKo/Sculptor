#pragma once

#include "Utilities/Macros.h"
#include "Types/InputAction.h"
#include "Types/KeyCode.h"

namespace Sculptor::Core
{
	class Input
	{
	public:
		static void Initialize();

		static bool IsKeyPressed(KeyCode keyCode);

		static bool IsKeyReleased(KeyCode keyCode);

		static bool IsKeyHeld(KeyCode keyCode);

		static void Update();

	private:
		Input();

		static Input* GetInstance();

		bool isKeyActionDirty{ true };

		static std::unique_ptr<Input> instance;

		std::array<InputAction, static_cast<int>(KeyCode::TOTAL_KEYS)> keyboardActionList;

		FRIEND(WindowsWindow)

		static void KeyCallback(int key, int scanCode, int action, int mods);

		static void UpdateKeyActionsList();
	};
}
