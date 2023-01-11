#pragma once

#include <entt.hpp>

namespace Pine
{
	class Scene
	{
	public:

		Scene();
		~Scene();

	private:

		entt::registry m_Registry;

	};
}