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

		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity);

	private:

		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

	private:

		Ref<Scene> m_Context;
		Entity m_SelectionContext = {};

	};
}
