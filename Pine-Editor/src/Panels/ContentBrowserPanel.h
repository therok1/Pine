#pragma once

#include <filesystem>

namespace Pine
{
	class ContentBrowserPanel
	{
	public:

		ContentBrowserPanel();

		void OnImGuiRender();

	private:

		std::filesystem::path m_CurrentDirectory;

	};
}
