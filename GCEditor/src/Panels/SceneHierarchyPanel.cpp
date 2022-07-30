#include "SceneHierarchyPanel.h"

#include <GCE/Scene/Components.h>

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace GCE
{

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		setContext(scene);
	}

	void SceneHierarchyPanel::setContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
	}

	void SceneHierarchyPanel::onImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_Context->m_Registry.each([&](auto entityID)
		{
			Entity entity{ entityID, m_Context.get() };
			drawEntityNode(entity);
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_SelectionContext)
			drawComponents(m_SelectionContext);

		ImGui::End();
	}

	void SceneHierarchyPanel::drawEntityNode(Entity entity)
	{
		auto& tag = entity.getComponent<TagComponent>().tag;
		
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		if (opened)
		{
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::drawComponents(Entity entity)
	{
		if (entity.hasComponent<TagComponent>())
		{
			auto& tag = entity.getComponent<TagComponent>();
			
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.tag.c_str());

			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.hasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.getComponent<TransformComponent>().transform;

				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.5f);

				ImGui::TreePop();
			}
		}

		if (entity.hasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& cameraComponent = entity.getComponent<CameraComponent>();
				auto& camera = cameraComponent.camera;

				ImGui::Checkbox("Primary", &cameraComponent.primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(uint32_t)camera.getProjectionType()];

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.setProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				if (camera.getProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.getOrthographicSize();
					float nearClip = camera.getOrthographicNearClip();
					float farClip = camera.getOrthographicFarClip();

					if (ImGui::DragFloat("Size", &orthoSize, 0.01f, 0.01f, 10.0f))
						camera.setOrthographicSize(orthoSize);

					if (ImGui::DragFloat("Near Clip", &nearClip))
						camera.setOrthographicNearClip(nearClip);

					if (ImGui::DragFloat("Far Clip", &farClip))
						camera.setOrthographicFarClip(farClip);

					if (ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.fixedAspectRatio))
						m_Context->resetAspectRatio();
				}

				if (camera.getProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float perspectiveFOV = glm::degrees(camera.getPerspectiveVerticalFOV());
					float nearClip = camera.getPerspectiveNearClip();
					float farClip = camera.getPerspectiveFarClip();

					if (ImGui::DragFloat("Vertical FOV", &perspectiveFOV))
						camera.setPerspectiveVerticalFOV(glm::radians(perspectiveFOV));

					if (ImGui::DragFloat("Near Clip", &nearClip))
						camera.setPerspectiveNearClip(nearClip);

					if (ImGui::DragFloat("Far Clip", &farClip))
						camera.setPerspectiveFarClip(farClip);
				}

				ImGui::TreePop();
			}
		}
	
		if (entity.hasComponent<SpriteRendererComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
			{
				auto& spriteRendererComponent = entity.getComponent<SpriteRendererComponent>();

				ImGui::ColorEdit4("Color", glm::value_ptr(spriteRendererComponent.color));

				ImGui::TreePop();
			}
		}
	}

}