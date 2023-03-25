#pragma once

namespace ARC
{
	struct SFileDialogs
	{
		[[nodiscard]] 
		static std::filesystem::path OpenFile(const char* _Filter);
		[[nodiscard]] 
		static std::filesystem::path SaveFile(const char* _Filter);
	};
}