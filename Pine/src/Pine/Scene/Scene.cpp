#include "pnpch.h"
#include "Scene.h"

#include <glm/glm.hpp>

namespace Pine
{
	Scene::Scene()
	{
		entt::entity entity = m_Registry.create();
	}

	Scene::~Scene()
	{

	}
}