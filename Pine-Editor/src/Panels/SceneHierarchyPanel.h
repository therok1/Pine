#pragma once

#include "Pine/Core/Core.h"
#include "Pine/Core/Log.h"
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

		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

	private:

		Ref<Scene> m_Context;
		Entity m_SelectionContext = {};

	};
}
