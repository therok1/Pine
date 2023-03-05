#pragma once

#include "Pine/Core/Core.h"
#include "Pine/Events/Event.h"

namespace Pine
{
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Pine Engine",
			uint32_t width = 1600,
			uint32_t height = 900)
			: Title(title), Width(width), Height(height)
		{

		}
	};

	class Window
	{
	public:

		using EventCallbackFn = std::function<void(Event&)>;

	public:

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}
