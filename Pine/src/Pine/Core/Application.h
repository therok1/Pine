#pragma once

#include "Pine/Core/Core.h"
#include "Pine/Core/Window.h"
#include "Pine/Core/LayerStack.h"
#include "Pine/Core/Timestep.h"

#include "Pine/Events/Event.h"
#include "Pine/Events/ApplicationEvent.h"

#include "Pine/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Pine
{
	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			PN_CORE_ASSERT(index < Count, "");
			return Args[index];
		}
	};

	class Application
	{
	public:

		Application(const std::string& name = "Pine Engine", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline static Application& Get() { return *s_Instance; }

		ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }

	private:

		bool OnWindowClosed(WindowCloseEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);

	private:

		ApplicationCommandLineArgs m_CommandLineArgs;

		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:

		static Application* s_Instance;

	};

	Application* CreateApplication(ApplicationCommandLineArgs args);
}

