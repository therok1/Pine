#include "pnpch.h"
#include "ScriptGlue.h"

#include "Pine/Scripting/ScriptEngine.h"

#include "Pine/Core/UUID.h"
#include "Pine/Core/KeyCodes.h"
#include "Pine/Core/Input.h"

#include "Pine/Scene/Scene.h"
#include "Pine/Scene/Entity.h"

#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>

#include <box2d/b2_body.h>

namespace Pine
{
	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;

#define PN_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Pine.InternalCalls::" #Name, Name)

	static void NativeLog(MonoString* string, int parameter)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);
		mono_free(cStr);
		std::cout << str << ", " << parameter << std::endl;
	}

	static void NativeLog_Vector(glm::vec3* parameter, glm::vec3* outResult)
	{
		PN_CORE_WARN("Value: {0}", *parameter);
		*outResult = glm::normalize(*parameter);
	}

	static float NativeLog_VectorDot(glm::vec3* parameter)
	{
		PN_CORE_WARN("Value: {0}", *parameter);
		return glm::dot(*parameter, *parameter);
	}

	static MonoObject* GetScriptInstance(UUID entityID)
	{
		return ScriptEngine::GetManagedInstance(entityID);
	}

	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		PN_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		PN_CORE_ASSERT(entity);

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		PN_CORE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end());
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}

	static uint64_t Entity_FindEntityByName(MonoString* name)
	{
		char* nameCStr = mono_string_to_utf8(name);

		Scene* scene = ScriptEngine::GetSceneContext();
		PN_CORE_ASSERT(scene);
		Entity entity = scene->FindEntityByName(nameCStr);
		mono_free(nameCStr);

		if (!entity)
			return 0;

		return entity.GetUUID();
	}

	static void TransformComponent_GetTranslation(UUID entityID, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		PN_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		PN_CORE_ASSERT(entity);

		*outTranslation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void TransformComponent_SetTranslation(UUID entityID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		PN_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		PN_CORE_ASSERT(entity);

		entity.GetComponent<TransformComponent>().Translation = *translation;
	}

	static void RigidBody2DComponent_ApplyLinearImpulse(UUID entityID, glm::vec2* impulse, glm::vec2* point, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		PN_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		PN_CORE_ASSERT(entity);

		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body* body = static_cast<b2Body*>(rb2d.RuntimeBody);
		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
	}

	static void RigidBody2DComponent_ApplyLinearImpulseToCenter(UUID entityID, glm::vec2* impulse, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		PN_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		PN_CORE_ASSERT(entity);
		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body* body = static_cast<b2Body*>(rb2d.RuntimeBody);
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}

	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}

	template<typename... Component>
	static void RegisterComponent()
	{
		([]()
			{
				std::string_view typeName = typeid(Component).name();
				size_t pos = typeName.find_last_of(':');
				std::string_view structName = typeName.substr(pos + 1);
				std::string managedTypename = fmt::format("Pine.{}", structName);

				MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetCoreAssemblyImage());
				if (!managedType)
				{
					PN_CORE_ERROR("Could not find component type {}", managedTypename);
					return;
				}
				s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
			}(), 
		...);
	}

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>)
	{
		RegisterComponent<Component...>();
	}

	void ScriptGlue::RegisterComponents()
	{
		RegisterComponent(AllComponents());
	}

	void ScriptGlue::RegisterFunctions()
	{
		PN_ADD_INTERNAL_CALL(NativeLog);
		PN_ADD_INTERNAL_CALL(NativeLog_Vector);
		PN_ADD_INTERNAL_CALL(NativeLog_VectorDot);

		PN_ADD_INTERNAL_CALL(GetScriptInstance);
		
		PN_ADD_INTERNAL_CALL(Entity_HasComponent);
		PN_ADD_INTERNAL_CALL(Entity_FindEntityByName);

		PN_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		PN_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);
		
		PN_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyLinearImpulse);
		PN_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyLinearImpulseToCenter);
		
		PN_ADD_INTERNAL_CALL(Input_IsKeyDown);
	}
}
