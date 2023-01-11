#pragma once

namespace Pine
{
	typedef enum class KeyCode : uint16_t
	{
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

#define PN_KEY_SPACE           ::Pine::Key::Space
#define PN_KEY_APOSTROPHE      ::Pine::Key::Apostrophe    /* ' */
#define PN_KEY_COMMA           ::Pine::Key::Comma         /* , */
#define PN_KEY_MINUS           ::Pine::Key::Minus         /* - */
#define PN_KEY_PERIOD          ::Pine::Key::Period        /* . */
#define PN_KEY_SLASH           ::Pine::Key::Slash         /* / */
#define PN_KEY_0               ::Pine::Key::D0
#define PN_KEY_1               ::Pine::Key::D1
#define PN_KEY_2               ::Pine::Key::D2
#define PN_KEY_3               ::Pine::Key::D3
#define PN_KEY_4               ::Pine::Key::D4
#define PN_KEY_5               ::Pine::Key::D5
#define PN_KEY_6               ::Pine::Key::D6
#define PN_KEY_7               ::Pine::Key::D7
#define PN_KEY_8               ::Pine::Key::D8
#define PN_KEY_9               ::Pine::Key::D9
#define PN_KEY_SEMICOLON       ::Pine::Key::Semicolon     /* ; */
#define PN_KEY_EQUAL           ::Pine::Key::Equal         /* = */
#define PN_KEY_A               ::Pine::Key::A
#define PN_KEY_B               ::Pine::Key::B
#define PN_KEY_C               ::Pine::Key::C
#define PN_KEY_D               ::Pine::Key::D
#define PN_KEY_E               ::Pine::Key::E
#define PN_KEY_F               ::Pine::Key::F
#define PN_KEY_G               ::Pine::Key::G
#define PN_KEY_H               ::Pine::Key::H
#define PN_KEY_I               ::Pine::Key::I
#define PN_KEY_J               ::Pine::Key::J
#define PN_KEY_K               ::Pine::Key::K
#define PN_KEY_L               ::Pine::Key::L
#define PN_KEY_M               ::Pine::Key::M
#define PN_KEY_N               ::Pine::Key::N
#define PN_KEY_O               ::Pine::Key::O
#define PN_KEY_P               ::Pine::Key::P
#define PN_KEY_Q               ::Pine::Key::Q
#define PN_KEY_R               ::Pine::Key::R
#define PN_KEY_S               ::Pine::Key::S
#define PN_KEY_T               ::Pine::Key::T
#define PN_KEY_U               ::Pine::Key::U
#define PN_KEY_V               ::Pine::Key::V
#define PN_KEY_W               ::Pine::Key::W
#define PN_KEY_X               ::Pine::Key::X
#define PN_KEY_Y               ::Pine::Key::Y
#define PN_KEY_Z               ::Pine::Key::Z
#define PN_KEY_LEFT_BRACKET    ::Pine::Key::LeftBracket   /* [ */
#define PN_KEY_BACKSLASH       ::Pine::Key::Backslash     /* \ */
#define PN_KEY_RIGHT_BRACKET   ::Pine::Key::RightBracket  /* ] */
#define PN_KEY_GRAVE_ACCENT    ::Pine::Key::GraveAccent   /* ` */
#define PN_KEY_WORLD_1         ::Pine::Key::World1        /* non-US #1 */
#define PN_KEY_WORLD_2         ::Pine::Key::World2        /* non-US #2 */

#define PN_KEY_ESCAPE          ::Pine::Key::Escape
#define PN_KEY_ENTER           ::Pine::Key::Enter
#define PN_KEY_TAB             ::Pine::Key::Tab
#define PN_KEY_BACKSPACE       ::Pine::Key::Backspace
#define PN_KEY_INSERT          ::Pine::Key::Insert
#define PN_KEY_DELETE          ::Pine::Key::Delete
#define PN_KEY_RIGHT           ::Pine::Key::Right
#define PN_KEY_LEFT            ::Pine::Key::Left
#define PN_KEY_DOWN            ::Pine::Key::Down
#define PN_KEY_UP              ::Pine::Key::Up
#define PN_KEY_PAGE_UP         ::Pine::Key::PageUp
#define PN_KEY_PAGE_DOWN       ::Pine::Key::PageDown
#define PN_KEY_HOME            ::Pine::Key::Home
#define PN_KEY_END             ::Pine::Key::End
#define PN_KEY_CAPS_LOCK       ::Pine::Key::CapsLock
#define PN_KEY_SCROLL_LOCK     ::Pine::Key::ScrollLock
#define PN_KEY_NUM_LOCK        ::Pine::Key::NumLock
#define PN_KEY_PRINT_SCREEN    ::Pine::Key::PrintScreen
#define PN_KEY_PAUSE           ::Pine::Key::Pause
#define PN_KEY_F1              ::Pine::Key::F1
#define PN_KEY_F2              ::Pine::Key::F2
#define PN_KEY_F3              ::Pine::Key::F3
#define PN_KEY_F4              ::Pine::Key::F4
#define PN_KEY_F5              ::Pine::Key::F5
#define PN_KEY_F6              ::Pine::Key::F6
#define PN_KEY_F7              ::Pine::Key::F7
#define PN_KEY_F8              ::Pine::Key::F8
#define PN_KEY_F9              ::Pine::Key::F9
#define PN_KEY_F10             ::Pine::Key::F10
#define PN_KEY_F11             ::Pine::Key::F11
#define PN_KEY_F12             ::Pine::Key::F12
#define PN_KEY_F13             ::Pine::Key::F13
#define PN_KEY_F14             ::Pine::Key::F14
#define PN_KEY_F15             ::Pine::Key::F15
#define PN_KEY_F16             ::Pine::Key::F16
#define PN_KEY_F17             ::Pine::Key::F17
#define PN_KEY_F18             ::Pine::Key::F18
#define PN_KEY_F19             ::Pine::Key::F19
#define PN_KEY_F20             ::Pine::Key::F20
#define PN_KEY_F21             ::Pine::Key::F21
#define PN_KEY_F22             ::Pine::Key::F22
#define PN_KEY_F23             ::Pine::Key::F23
#define PN_KEY_F24             ::Pine::Key::F24
#define PN_KEY_F25             ::Pine::Key::F25

#define PN_KEY_KP_0            ::Pine::Key::KP0
#define PN_KEY_KP_1            ::Pine::Key::KP1
#define PN_KEY_KP_2            ::Pine::Key::KP2
#define PN_KEY_KP_3            ::Pine::Key::KP3
#define PN_KEY_KP_4            ::Pine::Key::KP4
#define PN_KEY_KP_5            ::Pine::Key::KP5
#define PN_KEY_KP_6            ::Pine::Key::KP6
#define PN_KEY_KP_7            ::Pine::Key::KP7
#define PN_KEY_KP_8            ::Pine::Key::KP8
#define PN_KEY_KP_9            ::Pine::Key::KP9
#define PN_KEY_KP_DECIMAL      ::Pine::Key::KPDecimal
#define PN_KEY_KP_DIVIDE       ::Pine::Key::KPDivide
#define PN_KEY_KP_MULTIPLY     ::Pine::Key::KPMultiply
#define PN_KEY_KP_SUBTRACT     ::Pine::Key::KPSubtract
#define PN_KEY_KP_ADD          ::Pine::Key::KPAdd
#define PN_KEY_KP_ENTER        ::Pine::Key::KPEnter
#define PN_KEY_KP_EQUAL        ::Pine::Key::KPEqual

#define PN_KEY_LEFT_SHIFT      ::Pine::Key::LeftShift
#define PN_KEY_LEFT_CONTROL    ::Pine::Key::LeftControl
#define PN_KEY_LEFT_ALT        ::Pine::Key::LeftAlt
#define PN_KEY_LEFT_SUPER      ::Pine::Key::LeftSuper
#define PN_KEY_RIGHT_SHIFT     ::Pine::Key::RightShift
#define PN_KEY_RIGHT_CONTROL   ::Pine::Key::RightControl
#define PN_KEY_RIGHT_ALT       ::Pine::Key::RightAlt
#define PN_KEY_RIGHT_SUPER     ::Pine::Key::RightSuper
#define PN_KEY_MENU            ::Pine::Key::Menu