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

		static void Update();

		static bool IsKeyPressed(KeyCode keyCode);

		static bool IsKeyReleased(KeyCode keyCode);

		static bool IsKeyHeld(KeyCode keyCode);

		static void SetCursorHidden(bool value);

		static double GetMouseX();

		static double GetMouseY();

	private:
		bool isKeyActionDirty;

		static bool isCursorVisibilityDirty;

		static bool isCursorVisible;

		double mouseX;

		double mouseY;

		static std::unique_ptr<Input> instance;

		std::array<InputAction, static_cast<int>(KeyCode::TOTAL_KEYS)> keyboardActionList;

		FRIEND(WindowsWindow)

		Input();

		static Input* GetInstance();

		static void KeyCallback(int key, int scanCode, int action, int mods);

		static void MousePositionCallback(double xPosition, double yPosition);

		static void UpdateKeyActionsList();
	};
}
