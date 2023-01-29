#pragma once

#include "Pine/Scene/Scene.h"

#include <entt.hpp>

namespace Pine
{
	class Entity
	{
	public:

		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			PN_CORE_ASSERT(!HasComponent<T>(), "Component already exists!");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			PN_CORE_ASSERT(HasComponent<T>(), "Component doesn't exist!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			PN_CORE_ASSERT(HasComponent<T>(), "Component doesn't exist!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }

	private:

		entt::entity m_EntityHandle = entt::entity(entt::null);
		Scene* m_Scene = nullptr;

	};
}