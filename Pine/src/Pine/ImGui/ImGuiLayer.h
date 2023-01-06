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

		void Begin();
		void End();

	private:

		float m_Time = 0.0f;

	};
}