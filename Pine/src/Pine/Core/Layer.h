#pragma once

#include "Pine/Core/Core.h"
#include "Pine/Core/Timestep.h"
#include "Pine/Events/Event.h"

namespace Pine
{
	class Layer
	{
	public:

		Layer(const std::string& debugName = "Layer");
		virtual ~Layer();

		virtual void OnAttach()
		{

		}

		virtual void OnDetach()
		{

		}

		virtual void OnUpdate(Timestep ts)
		{

		}

		virtual void OnImGuiRender()
		{

		}

		virtual void OnEvent(Event& event)
		{

		}

		inline const std::string& GetName() const
		{
			return m_DebugName;
		}

	protected:

		std::string m_DebugName;

	};
}