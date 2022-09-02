#pragma once
#include <string>

namespace ARC
{
	struct CFileDialogs
	{
		[[nodiscard]] static std::string OpenFile(const char* _Filter);
		[[nodiscard]] static std::string SaveFile(const char* _Filter);
	};
}