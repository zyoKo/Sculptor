#pragma once

namespace Sculptor::Core
{
	enum class KeyCode : std::uint8_t
	{
		KEY_NONE = -1,

		KEY_A = 0,
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_E,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_I,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_M,
		KEY_N,
		KEY_O,
		KEY_P,
		KEY_Q,
		KEY_R,
		KEY_S,
		KEY_T,
		KEY_U,
		KEY_V,
		KEY_W,
		KEY_X,
		KEY_Y,
		KEY_Z,

		KEY_0,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,

		KEY_F1,
		KEY_F2, 
		KEY_F3, 
		KEY_F4, 
		KEY_F5, 
		KEY_F6, 
		KEY_F7, 
		KEY_F8, 
		KEY_F9, 
		KEY_F10,
		KEY_F11,
		KEY_F12,

		KEY_LEFT_SHIFT,
		KEY_LEFT_CONTROL,
		KEY_LEFT_ALT,
		KEY_LEFT_SUPER,		/* Left WINDOWS/MAC(CMD) Key */
		KEY_RIGHT_SHIFT,
		KEY_RIGHT_CONTROL,
		KEY_RIGHT_ALT,
		KEY_RIGHT_SUPER,	/* Right WINDOWS/MAC(CMD) Key */

		KEY_ESCAPE,
		KEY_ENTER,
		KEY_TAB,
		KEY_BACKSPACE,
		KEY_INSERT,
		KEY_DELETE,
		KEY_RIGHT,		/* Arrow Keys */
		KEY_LEFT,
		KEY_DOWN,
		KEY_UP,
		KEY_PAGE_UP,
		KEY_PAGE_DOWN,
		KEY_HOME,
		KEY_END,
		KEY_CAPS_LOCK,
		KEY_SCROLL_LOCK,
		KEY_NUM_LOCK,
		KEY_PRINT_SCREEN,
		KEY_PAUSE,

		KEY_SPACE,   
		KEY_APOSTROPHE,
		KEY_COMMA,
		KEY_MINUS,
		KEY_PERIOD,
		KEY_SLASH,

		KEY_SEMICOLON,
		KEY_EQUAL,

		TOTAL_KEYS
	};

	KeyCode GlfwKeyToEngineKeyCode(int glfwKey);

	void PrintKeyUsed(KeyCode keyCode);
}
