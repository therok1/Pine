#pragma once

#include "Pine/Core/Layer.h"

#include "Pine/Events/ApplicationEvent.h"
#include "Pine/Events/KeyEvent.h"
#include "Pine/Events/MouseEvent.h"

namespace Pine
{
	class ImGuiLayer : public Layer
	{
	public:

		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

		void BlockEvents(bool blockEvents) { m_BlockEvents = blockEvents; }

		void SetDarkThemeColors();

	private:

		bool m_BlockEvents = true;
		float m_Time = 0.0f;

	};
}
