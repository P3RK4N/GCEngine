#pragma once

#include <GCE/Renderer/Texture.h>

namespace GCE
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void onImGuiRender();

	private:
		std::filesystem::path m_CurrentDirectory;

		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
	};

}
