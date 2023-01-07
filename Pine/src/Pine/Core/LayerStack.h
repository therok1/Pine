#pragma once

#include "Pine/Core/Core.h"
#include "Pine/Core/Layer.h"

namespace Pine
{
	class LayerStack
	{
	public:

		LayerStack() = default;
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin()
		{
			return m_Layers.begin();
		}

		std::vector<Layer*>::iterator end()
		{
			return m_Layers.end();
		}

	private:

		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsert = 0;

	};
}