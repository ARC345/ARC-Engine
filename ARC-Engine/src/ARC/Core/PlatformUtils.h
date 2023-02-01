#pragma once
#include <string>

namespace ARC
{
	struct CFileDialogs
	{
		[[nodiscard]] 
		static TString OpenFile(const char* _Filter);
		[[nodiscard]] 
		static TString SaveFile(const char* _Filter);
	};
}