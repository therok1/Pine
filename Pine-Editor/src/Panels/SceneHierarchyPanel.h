#pragma once

#include "Pine/Core/Core.h"
#include "Pine/Scene/Scene.h"
#include "Pine/Scene/Entity.h"

namespace Pine
{
	class SceneHierarchyPanel
	{
	public:

		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();

	private:

		Ref<Scene> m_Context;

	};
}