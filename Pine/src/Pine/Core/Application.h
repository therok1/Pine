#pragma once

#include "Pine/Core/Core.h"
#include "Pine/Core/Window.h"
#include "Pine/Core/LayerStack.h"
#include "Pine/Core/Timestep.h"

#include "Pine/Events/Event.h"
#include "Pine/Events/ApplicationEvent.h"

#include "Pine/ImGui/ImGuiLayer.h"

namespace Pine
{
	class Application
	{
	public:

		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow()
		{
			return *m_Window;
		}

		inline static Application& Get()
		{
			return *s_Instance;
		}

	private:

		bool OnWindowClosed(WindowCloseEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);

	private:

		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:

		static Application* s_Instance;

	};

	Application* CreateApplication();
}
