#pragma once

#include "Pine/Project/Project.h"

namespace Pine
{
	class ProjectSerializer
	{
	public:

		ProjectSerializer(Ref<Project> project);

		bool Serialize(const std::filesystem::path& filepath);
		bool Deserialize(const std::filesystem::path& filepath);

	private:

		Ref<Project> m_Project;

	};
}
