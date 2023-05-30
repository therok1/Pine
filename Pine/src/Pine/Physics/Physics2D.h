#pragma once

#include "Pine/Scene/Components.h"

#include <box2d/b2_body.h>

namespace Pine
{
	namespace Utils
	{
		inline b2BodyType RigidBody2DTypeToBox2DBody(RigidBody2DComponent::BodyType bodyType)
		{
			switch (bodyType)
			{
			case RigidBody2DComponent::BodyType::Static:	return b2_staticBody;
			case RigidBody2DComponent::BodyType::Dynamic:	return b2_dynamicBody;
			case RigidBody2DComponent::BodyType::Kinematic:	return b2_kinematicBody;
			}

			PN_CORE_ASSERT(false, "Invalid body type!");
			return b2_staticBody;
		}

		inline RigidBody2DComponent::BodyType RigidBody2DTypeFromBox2DBody(b2BodyType bodyType)
		{
			switch (bodyType)
			{
			case b2_staticBody: return RigidBody2DComponent::BodyType::Static;
			case b2_dynamicBody: return RigidBody2DComponent::BodyType::Dynamic;
			case b2_kinematicBody: return RigidBody2DComponent::BodyType::Kinematic;
			}

			PN_CORE_ASSERT(false, "Invalid body type!");
			return RigidBody2DComponent::BodyType::Static;
		}
	}
}
