#include "GCEPCH.h"
#include "ContentBrowserPanel.h"

#include <imgui.h>
#include <filesystem>

namespace  GCE
{
	extern const std::filesystem::path g_AssetPath;


	ContentBrowserPanel::ContentBrowserPanel()
	{
		m_CurrentDirectory = g_AssetPath;
		m_DirectoryIcon = Texture2D::create("resources/icons/folder.png");
		m_FileIcon = Texture2D::create("resources/icons/file.png");
	}

	void ContentBrowserPanel::onImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != g_AssetPath)
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		columnCount = std::max(1, columnCount);

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, g_AssetPath);
			std::string filenameString = relativePath.filename().string();
			std::string fileExtension = relativePath.filename().extension().string();

			ImGui::PushID(filenameString.c_str());

			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0,0,0,0});
			ImGui::ImageButton((ImTextureID)icon->getRendererID(), { thumbnailSize, thumbnailSize}, { 0, 1 }, { 1, 0 });
			;
			if (fileExtension == ".gce")
			{
				if (ImGui::BeginDragDropSource())
				{
					const wchar_t* itemPath = relativePath.c_str();
					ImGui::SetDragDropPayload("SCENE_ASSET", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
					ImGui::EndDragDropSource();
				}
			}
			else
			{
				if (ImGui::BeginDragDropSource())
				{
					const wchar_t* itemPath = relativePath.c_str();
					ImGui::SetDragDropPayload("TEXTURE_ASSET", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
					ImGui::EndDragDropSource();
				}
			}

			ImGui::PopStyleColor();

			if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if(directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();
			}

			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);
		ImGui::End();
	}

}
