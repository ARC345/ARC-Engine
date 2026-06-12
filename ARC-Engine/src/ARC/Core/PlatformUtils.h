#pragma once

namespace arc
{
	struct file_dialogs
	{
		[[nodiscard]] 
		static std::filesystem::path open_file(const char* filter);
		[[nodiscard]] 
		static std::filesystem::path save_file(const char* filter);
	};
}