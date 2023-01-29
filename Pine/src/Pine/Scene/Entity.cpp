#include "pnpch.h"
#include "Entity.h"

namespace Pine
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{

	}
}