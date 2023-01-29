#pragma once

#include "Pine/Core/Timestep.h"

#include <entt.hpp>


namespace Pine
{
	class Entity;

	class Scene
	{
	public:

		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		void OnUpdate(Timestep ts);

	private:

		entt::registry m_Registry;

		friend class Entity;

	};
}