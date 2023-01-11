#pragma once

namespace Pine
{
	typedef enum class MouseButtonCode : uint16_t
	{
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseButtonCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define PN_MOUSE_BUTTON_0         ::Pine::Mouse::Button0
#define PN_MOUSE_BUTTON_1         ::Pine::Mouse::Button1
#define PN_MOUSE_BUTTON_2         ::Pine::Mouse::Button2
#define PN_MOUSE_BUTTON_3         ::Pine::Mouse::Button3
#define PN_MOUSE_BUTTON_4         ::Pine::Mouse::Button4
#define PN_MOUSE_BUTTON_5         ::Pine::Mouse::Button5
#define PN_MOUSE_BUTTON_6         ::Pine::Mouse::Button6
#define PN_MOUSE_BUTTON_7         ::Pine::Mouse::Button7
#define PN_MOUSE_BUTTON_LAST      ::Pine::Mouse::ButtonLast
#define PN_MOUSE_BUTTON_LEFT      ::Pine::Mouse::ButtonLeft
#define PN_MOUSE_BUTTON_RIGHT     ::Pine::Mouse::ButtonRight
#define PN_MOUSE_BUTTON_MIDDLE    ::Pine::Mouse::ButtonMiddle