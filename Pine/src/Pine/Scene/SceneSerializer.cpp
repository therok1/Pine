#include "pnpch.h"
#include "SceneSerializer.h"

#include "Pine/Scene/Entity.h"
#include "Pine/Scene/Components.h"
#include "Pine/Scripting/ScriptEngine.h"
#include "Pine/Core/UUID.h"

#include <yaml-cpp/yaml.h>

namespace YAML 
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<Pine::UUID>
	{
		static Node encode(const Pine::UUID& uuid)
		{
			Node node;
			node.push_back(static_cast<uint64_t>(uuid));
			return node;
		}

		static bool decode(const Node& node, Pine::UUID& uuid)
		{
			uuid = node.as<uint64_t>();
			return true;
		}
	};

}

namespace Pine
{
#define WRITE_SCRIPT_FIELD(FieldType, Type)				\
			case ScriptFieldType::FieldType:			\
				out << scriptField.GetValue<Type>();	\
				break

#define READ_SCRIPT_FIELD(FieldType, Type)				\
	case ScriptFieldType::FieldType:					\
	{													\
		Type data = scriptField["Data"].as<Type>();		\
		fieldInstance.SetValue(data);					\
		break;											\
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	static std::string RigidBody2DBodyTypeToString(RigidBody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
		case RigidBody2DComponent::BodyType::Static:	return "Static";
		case RigidBody2DComponent::BodyType::Dynamic:	return "Dynamic";
		case RigidBody2DComponent::BodyType::Kinematic:	return "Kinematic";
		}

		PN_CORE_ASSERT(false, "Invalid body type!");
		return std::string();
	}

	static RigidBody2DComponent::BodyType RigidBody2DBodyTypeFromString(const std::string& bodyTypeString)
	{
		if (bodyTypeString == "Static")		return RigidBody2DComponent::BodyType::Static;
		if (bodyTypeString == "Dynamic")	return RigidBody2DComponent::BodyType::Dynamic;
		if (bodyTypeString == "Kinematic")	return RigidBody2DComponent::BodyType::Kinematic;

		PN_CORE_ASSERT(false, "Invalid body type!");
		return RigidBody2DComponent::BodyType::Static;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{

	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		PN_CORE_ASSERT(entity.HasComponent<IDComponent>(), "Entity doesn't have an IDComponent!");

		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();
		
		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto& transformComponent = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << transformComponent.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << transformComponent.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << transformComponent.Scale;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;

			out << YAML::Key << "ProjectionType" << YAML::Value << static_cast<int>(camera.GetProjectionType());
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();

			out << YAML::EndMap;

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;

			auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;

			if (spriteRendererComponent.Texture)
				out << YAML::Key << "TexturePath" << YAML::Value << spriteRendererComponent.Texture->GetPath();

			out << YAML::Key << "TilingFactor" << YAML::Value << spriteRendererComponent.TilingFactor;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CircleRendererComponent>())
		{
			out << YAML::Key << "CircleRendererComponent";
			out << YAML::BeginMap;

			auto& circleRendererComponent = entity.GetComponent<CircleRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << circleRendererComponent.Color;
			out << YAML::Key << "Thickness" << YAML::Value << circleRendererComponent.Thickness;
			out << YAML::Key << "Fade" << YAML::Value << circleRendererComponent.Fade;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<ScriptComponent>())
		{

			out << YAML::Key << "ScriptComponent";
			out << YAML::BeginMap;

			auto& scriptComponent = entity.GetComponent<ScriptComponent>();
			out << YAML::Key << "ClassName" << YAML::Value << scriptComponent.ClassName;

			Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(scriptComponent.ClassName);
			const auto& fields = entityClass->GetFields();
			if (fields.size() > 0)
			{
				out << YAML::Key << "ScriptFields" << YAML::Value;
				auto& entityFields = ScriptEngine::GetScriptFieldMap(entity);
				out << YAML::BeginSeq;
				for (const auto& [name, field] : fields)
				{
					if (entityFields.find(name) == entityFields.end())
						continue;

					out << YAML::BeginMap;
					out << YAML::Key << "Name" << YAML::Value << name;
					out << YAML::Key << "Type" << YAML::Value << Utils::ScriptFieldTypeToString(field.Type);

					out << YAML::Key << "Data" << YAML::Value;
					ScriptFieldInstance& scriptField = entityFields.at(name);

					switch (field.Type)
					{
						WRITE_SCRIPT_FIELD(Float,		float);
						WRITE_SCRIPT_FIELD(Double,		double);
						WRITE_SCRIPT_FIELD(Bool,		bool);
						WRITE_SCRIPT_FIELD(Char,		char);
						WRITE_SCRIPT_FIELD(Byte,		int8_t);
						WRITE_SCRIPT_FIELD(Short,		int16_t);
						WRITE_SCRIPT_FIELD(Int,			int32_t);
						WRITE_SCRIPT_FIELD(Long,		int64_t);
						WRITE_SCRIPT_FIELD(UByte,		uint8_t);
						WRITE_SCRIPT_FIELD(UShort,		uint16_t);
						WRITE_SCRIPT_FIELD(UInt,		uint32_t);
						WRITE_SCRIPT_FIELD(ULong,		uint64_t);
						WRITE_SCRIPT_FIELD(Vector2,		glm::vec2);
						WRITE_SCRIPT_FIELD(Vector3,		glm::vec3);
						WRITE_SCRIPT_FIELD(Vector4,		glm::vec4);
						WRITE_SCRIPT_FIELD(Entity,		UUID);
					}

					out << YAML::EndMap;
				}

				out << YAML::EndSeq;
			}

			out << YAML::EndMap;
		}

		if (entity.HasComponent<RigidBody2DComponent>())
		{
			out << YAML::Key << "RigidBody2DComponent";
			out << YAML::BeginMap;

			auto& rb2dComponent = entity.GetComponent<RigidBody2DComponent>();
			out << YAML::Key << "BodyType" << YAML::Value << RigidBody2DBodyTypeToString(rb2dComponent.Type);
			out << YAML::Key << "FixedRotation" << YAML::Value << rb2dComponent.FixedRotation;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap;

			auto& bc2dComponent = entity.GetComponent<BoxCollider2DComponent>();
			out << YAML::Key << "Offset" << YAML::Value << bc2dComponent.Offset;
			out << YAML::Key << "Size" << YAML::Value << bc2dComponent.Size;
			out << YAML::Key << "Density" << YAML::Value << bc2dComponent.Density;
			out << YAML::Key << "Friction" << YAML::Value << bc2dComponent.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << bc2dComponent.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << bc2dComponent.RestitutionThreshold;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			out << YAML::Key << "CircleCollider2DComponent";
			out << YAML::BeginMap;

			auto& cc2dComponent = entity.GetComponent<CircleCollider2DComponent>();
			out << YAML::Key << "Offset" << YAML::Value << cc2dComponent.Offset;
			out << YAML::Key << "Radius" << YAML::Value << cc2dComponent.Radius;
			out << YAML::Key << "Density" << YAML::Value << cc2dComponent.Density;
			out << YAML::Key << "Friction" << YAML::Value << cc2dComponent.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << cc2dComponent.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << cc2dComponent.RestitutionThreshold;

			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each(
			[&](auto entityID)
			{
				Entity entity(entityID, m_Scene.get());
				if (!entity)
					return;

				SerializeEntity(out, entity);
			}
		);
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		PN_CORE_ASSERT(false, "Not implemented!");
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath);
		}
		catch (const std::exception& e)
		{
			PN_CORE_ERROR("Failed to load .pine file '{0}'\n     {1}", filepath, e.what());
			return false;
		}

		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		PN_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				PN_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntityWithUUID(uuid, name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& transformComponent_ = deserializedEntity.GetComponent<TransformComponent>();
					transformComponent_.Translation = transformComponent["Translation"].as<glm::vec3>();
					transformComponent_.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					transformComponent_.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cameraComponent_ = deserializedEntity.AddComponent<CameraComponent>();

					auto& cameraProps = cameraComponent["Camera"];
					cameraComponent_.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cameraComponent_.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
					cameraComponent_.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cameraComponent_.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cameraComponent_.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cameraComponent_.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cameraComponent_.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cameraComponent_.Primary = cameraComponent["Primary"].as<bool>();
					cameraComponent_.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& spriteRendererComponent_ = deserializedEntity.AddComponent<SpriteRendererComponent>();
					spriteRendererComponent_.Color = spriteRendererComponent["Color"].as<glm::vec4>();

					if (spriteRendererComponent["TexturePath"])
						spriteRendererComponent_.Texture = Texture2D::Create(spriteRendererComponent["TexturePath"].as<std::string>());

					if (spriteRendererComponent["TilingFactor"])
						spriteRendererComponent_.TilingFactor = spriteRendererComponent["TilingFactor"].as<float>();
				}

				auto circleRendererComponent = entity["CircleRendererComponent"];
				if (circleRendererComponent)
				{
					auto& circleRendererComponent_ = deserializedEntity.AddComponent<CircleRendererComponent>();
					circleRendererComponent_.Color = circleRendererComponent["Color"].as<glm::vec4>();
					circleRendererComponent_.Thickness = circleRendererComponent["Thickness"].as<float>();
					circleRendererComponent_.Fade = circleRendererComponent["Fade"].as<float>();
				}

				auto scriptComponent = entity["ScriptComponent"];
				if (scriptComponent)
				{
					auto& scriptComponent_ = deserializedEntity.AddComponent<ScriptComponent>();
					scriptComponent_.ClassName = scriptComponent["ClassName"].as<std::string>();

					auto scriptFields = scriptComponent["ScriptFields"];
					if (scriptFields)
					{
						Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(scriptComponent_.ClassName);
						PN_CORE_ASSERT(entityClass);
						const auto& fields = entityClass->GetFields();
						auto& entityFields = ScriptEngine::GetScriptFieldMap(deserializedEntity);

						for (auto scriptField : scriptFields)
						{
							std::string name = scriptField["Name"].as<std::string>();
							std::string typeString = scriptField["Type"].as<std::string>();
							ScriptFieldType type = Utils::ScriptFieldTypeFromString(typeString);

							ScriptFieldInstance& fieldInstance = entityFields[name];

							PN_CORE_ASSERT(fields.find(name) != fields.end());

							if (fields.find(name) == fields.end())
								continue;

							fieldInstance.Field = fields.at(name);

							switch (type)
							{
								READ_SCRIPT_FIELD(Float,		float);
								READ_SCRIPT_FIELD(Double,		double);
								READ_SCRIPT_FIELD(Bool,			bool);
								READ_SCRIPT_FIELD(Char,			char);
								READ_SCRIPT_FIELD(Byte,			int8_t);
								READ_SCRIPT_FIELD(Short,		int16_t);
								READ_SCRIPT_FIELD(Int,			int32_t);
								READ_SCRIPT_FIELD(Long,			int64_t);
								READ_SCRIPT_FIELD(UByte,		uint8_t);
								READ_SCRIPT_FIELD(UShort,		uint16_t);
								READ_SCRIPT_FIELD(UInt,			uint32_t);
								READ_SCRIPT_FIELD(ULong,		uint64_t);
								READ_SCRIPT_FIELD(Vector2,		glm::vec2);
								READ_SCRIPT_FIELD(Vector3,		glm::vec3);
								READ_SCRIPT_FIELD(Vector4,		glm::vec4);
								READ_SCRIPT_FIELD(Entity,		UUID);
							}
						}
					}
				}

				auto rigidBody2DComponent = entity["RigidBody2DComponent"];
				if (rigidBody2DComponent)
				{
					auto& rigidBody2DComponent_ = deserializedEntity.AddComponent<RigidBody2DComponent>();
					rigidBody2DComponent_.Type = RigidBody2DBodyTypeFromString(rigidBody2DComponent["BodyType"].as<std::string>());
					rigidBody2DComponent_.FixedRotation = rigidBody2DComponent["FixedRotation"].as<bool>();
				}

				auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
				if (boxCollider2DComponent)
				{
					auto& boxCollider2DComponent_ = deserializedEntity.AddComponent<BoxCollider2DComponent>();
					boxCollider2DComponent_.Offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
					boxCollider2DComponent_.Size = boxCollider2DComponent["Size"].as<glm::vec2>();
					boxCollider2DComponent_.Density = boxCollider2DComponent["Density"].as<float>();
					boxCollider2DComponent_.Friction = boxCollider2DComponent["Friction"].as<float>();
					boxCollider2DComponent_.Restitution = boxCollider2DComponent["Restitution"].as<float>();
					boxCollider2DComponent_.RestitutionThreshold = boxCollider2DComponent["RestitutionThreshold"].as<float>();
				}

				auto circleCollider2DComponent = entity["CircleCollider2DComponent"];
				if (circleCollider2DComponent)
				{
					auto& circleCollider2DComponent_ = deserializedEntity.AddComponent<CircleCollider2DComponent>();
					circleCollider2DComponent_.Offset = circleCollider2DComponent["Offset"].as<glm::vec2>();
					circleCollider2DComponent_.Radius = circleCollider2DComponent["Radius"].as<float>();
					circleCollider2DComponent_.Density = circleCollider2DComponent["Density"].as<float>();
					circleCollider2DComponent_.Friction = circleCollider2DComponent["Friction"].as<float>();
					circleCollider2DComponent_.Restitution = circleCollider2DComponent["Restitution"].as<float>();
					circleCollider2DComponent_.RestitutionThreshold = circleCollider2DComponent["RestitutionThreshold"].as<float>();
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		PN_CORE_ASSERT(false, "Not implemented!");
		return false;
	}
}
