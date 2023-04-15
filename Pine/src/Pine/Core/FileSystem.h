#pragma once

#include "Pine/Core/Buffer.h"

namespace Pine 
{
	class FileSystem
	{
	public:
		
		static Buffer ReadFileBinary(const std::filesystem::path& filepath);

	};
}
