#include "GCEPCH.h"
#include "GCE/Scene/SceneSerializer.h"

#include "GCE/Scene/Entity.h"
#include "GCE/Scene/Components.h"
#include "GCE/Renderer/Texture.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include <fstream>

#include <yaml-cpp/yaml.h>

#include "Components.h"

namespace YAML {

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

}

namespace GCE
{
	static std::string RigidBody2DBodyTypeToString(Rigidbody2DComponent::BodyType type)
	{
		switch (type)
		{
			case Rigidbody2DComponent::BodyType::Static:	return "Static";
			case Rigidbody2DComponent::BodyType::Dynamic:	return "Dynamic";
			case Rigidbody2DComponent::BodyType::Kinematic: return "Kinematic";
		}

		GCE_CORE_ASSERT(false, "Unknown body type");
		return "";
	}

	static Rigidbody2DComponent::BodyType RigidBody2DBodyTypeFromString(const std::string& type)
	{
		if (type == "Static")		return Rigidbody2DComponent::BodyType::Static;
		if (type == "Dynamic")		return Rigidbody2DComponent::BodyType::Dynamic;
		if (type == "Kinematic")	return Rigidbody2DComponent::BodyType::Kinematic;

		GCE_CORE_ASSERT(false, "Unknown body type");
		return Rigidbody2DComponent::BodyType::Static;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& vec)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& vec)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& vec)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << vec.z << vec.w << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const SceneCamera& camera)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.getProjectionType();
		out << YAML::Key << "OrthographicSize" << YAML::Value << camera.getOrthographicSize();
		out << YAML::Key << "OrthographicNear" << YAML::Value << camera.getOrthographicNearClip();
		out << YAML::Key << "OrthographicFar" << YAML::Value << camera.getOrthographicFarClip();
		out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.getPerspectiveVerticalFOV();
		out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.getPerspectiveNearClip();
		out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.getPerspectiveFarClip();
		out << YAML::EndMap;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene) :
		m_Scene(scene)
	{
		
	}

	template<typename T, typename Function>
	static void serializeComponent(const std::string& componentName, YAML::Emitter& out, Entity entity, Function func)
	{
		if (entity.hasComponent<T>())
		{
			out << YAML::Key << componentName; //Upitno
			out << YAML::BeginMap; //TagComponent

			auto& component = entity.getComponent<T>();
			func(out, component);

			out << YAML::EndMap; //TagComponent
		}
	}

	static void serializeEntity(YAML::Emitter& out, Entity entity)
	{
		GCE_ASSERT(entity.hasComponent<IDComponent>(), "Unknown entity");

		out << YAML::BeginMap; //Entity
		out << YAML::Key << "Entity" << YAML::Value << entity.getUUID();

		serializeComponent<TagComponent>("TagComponent", out, entity, [](YAML::Emitter& out, const TagComponent& component)
		{
			out << YAML::Key << "Tag" << YAML::Value << component.tag;
		});

		serializeComponent<TransformComponent>("TransformComponent", out, entity, [](YAML::Emitter& out, const TransformComponent& component)
		{
			out << YAML::Key << "Position" << YAML::Value << component.translation;
			out << YAML::Key << "Rotation" << YAML::Value << component.rotation;
			out << YAML::Key << "Scale" << YAML::Value << component.scale;
		});

		serializeComponent<SpriteRendererComponent>("SpriteRendererComponent", out, entity, [](YAML::Emitter& out, const SpriteRendererComponent& component)
		{
			out << YAML::Key << "Color" << YAML::Value << component.color;
			out << YAML::Key << "Texture" << YAML::Value << (component.texture ? (std::reinterpret_pointer_cast<OpenGLTexture2D>(component.texture))->getPath() : "");
			out << YAML::Key << "TextureScale" << YAML::Value << component.textureScale;
		});

		serializeComponent<CircleRendererComponent>("CircleRendererComponent", out, entity, [](YAML::Emitter& out, const CircleRendererComponent& component)
		{
			out << YAML::Key << "Color" << YAML::Value << component.color;
			out << YAML::Key << "Thickness" << YAML::Value << component.thickness;
			out << YAML::Key << "Fade" << YAML::Value << component.fade;
		});

		serializeComponent<CameraComponent>("CameraComponent", out, entity, [](YAML::Emitter& out, const CameraComponent& component)
		{
			out << YAML::Key << "Camera" << YAML::Value << component.camera;
			out << YAML::Key << "Primary" << YAML::Value << component.primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << component.fixedAspectRatio;
		});

		serializeComponent<Rigidbody2DComponent>("Rigidbody2DComponent", out, entity, [](YAML::Emitter& out, const Rigidbody2DComponent& component)
		{
			out << YAML::Key << "BodyType" << YAML::Value << RigidBody2DBodyTypeToString(component.type);
			out << YAML::Key << "FixedRotation" << YAML::Value << component.fixedRotation;
		});

		serializeComponent<BoxCollider2DComponent>("BoxCollider2DComponent", out, entity, [](YAML::Emitter& out, const BoxCollider2DComponent& component)
		{
			out << YAML::Key << "Offset" << YAML::Value << component.offset;
			out << YAML::Key << "Size" << YAML::Value << component.size;
			out << YAML::Key << "Density" << YAML::Value << component.density;
			out << YAML::Key << "Friction" << YAML::Value << component.friction;
			out << YAML::Key << "Restitution" << YAML::Value << component.restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << component.restitutionThreshold;
		});

		out << YAML::EndMap; //Entity
	}

	void SceneSerializer::serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene";
		out << YAML::Value << "Untitled";
		out << YAML::Key << "Entities";
		out << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID)
		{
				Entity entity = { entityID, m_Scene.get() };
				if (!entity)
					return;
				serializeEntity(out, entity);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::serializeRuntime(const std::string& filepath)
	{
		//TODO(P3RK4N): Implement
	}

	bool SceneSerializer::deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		GCE_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if(entities)
		{
			for(auto entity : entities)
			{
				std::string name;
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				GCE_CORE_TRACE("Deserializing entity '{0}'", name);

				Entity deserializedEntity = m_Scene->createEntityWithUUID(uuid, name);

				auto transformComponent = entity["TransformComponent"];
				if(transformComponent)
				{
					auto& tc = deserializedEntity.getComponent<TransformComponent>();
					tc.translation = transformComponent["Position"].as<glm::vec3>();
					tc.rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.addComponent<CameraComponent>();

					auto cameraProps = cameraComponent["Camera"];

					cc.camera.setProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.camera.setPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.camera.setPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.camera.setPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.camera.setOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.camera.setOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.camera.setOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.primary = cameraComponent["Primary"].as<bool>();
					cc.fixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& src = deserializedEntity.addComponent<SpriteRendererComponent>();
					src.color = spriteRendererComponent["Color"].as<glm::vec4>();
					src.textureScale = spriteRendererComponent["TextureScale"].as<float>();
					auto texturePath = spriteRendererComponent["Texture"].as<std::string>();
					if (!texturePath.empty())
						src.texture = Texture2D::create(texturePath);
				}

				auto circleRendererComponent = entity["CircleRendererComponent"];
				if (circleRendererComponent)
				{
					auto& crc = deserializedEntity.addComponent<CircleRendererComponent>();
					crc.color = circleRendererComponent["Color"].as<glm::vec4>();
					crc.thickness = circleRendererComponent["Thickness"].as<float>();
					crc.fade= circleRendererComponent["Fade"].as<float>();
				}

				auto rigidbody2DComponent = entity["Rigidbody2DComponent"];
				if (rigidbody2DComponent)
				{
					auto& rb2dc = deserializedEntity.addComponent<Rigidbody2DComponent>();
					rb2dc.type = RigidBody2DBodyTypeFromString(rigidbody2DComponent["BodyType"].as<std::string>());
					rb2dc.fixedRotation = rigidbody2DComponent["FixedRotation"].as<bool>();
				}

				auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
				if (boxCollider2DComponent)
				{
					auto& bc2dc = deserializedEntity.addComponent<BoxCollider2DComponent>();
					bc2dc.size = boxCollider2DComponent["Size"].as<glm::vec2>();
					bc2dc.offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
					bc2dc.density = boxCollider2DComponent["Density"].as<float>();
					bc2dc.friction = boxCollider2DComponent["Friction"].as<float>();
					bc2dc.restitution = boxCollider2DComponent["Restitution"].as<float>();
					bc2dc.restitutionThreshold = boxCollider2DComponent["RestitutionThreshold"].as<float>();
				}
			}
		}

		return true;
	}

	bool SceneSerializer::deserializeRuntime(const std::string& filepath)
	{
		//TODO(P3RK4N): Implement
		return false;
	}

}