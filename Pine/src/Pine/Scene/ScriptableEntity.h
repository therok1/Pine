#pragma once

#include "Pine/Scene/Entity.h"

namespace Pine
{
	class ScriptableEntity
	{
	public:

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	private:

		Entity m_Entity;
		friend class Scene;

	};
}