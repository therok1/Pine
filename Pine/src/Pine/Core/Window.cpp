#include "pnpch.h"
#include "Window.h"

#ifdef PN_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Pine
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef PN_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		PN_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}
}