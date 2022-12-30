#pragma once

#include "Pine/Layer.h"

namespace Pine
{
	class PINE_API ImGuiLayer : public Layer
	{
	public:

		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& event) override;

	private:

		float m_Time = 0.0f;

	};
}