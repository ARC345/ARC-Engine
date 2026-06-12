#include "arc_pch.h"
#include "ARC/Core/PlatformUtils.h"
#include "ARC/Core/Application.h"

#include <cstdio>
#include <memory>
#include <array>
#include <string>
#include <cstdlib>

namespace ARC
{
	// Execute a shell command and capture stdout
	static std::string ExecCommand(const char* pCmd)
	{
		std::array<char, 256> buffer;
		std::string result;
		std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(pCmd, "r"), pclose);
		if (!pipe)
			return "";
		while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
			result += buffer.data();
		// Trim trailing newline
		if (!result.empty() && result.back() == '\n')
			result.pop_back();
		return result;
	}

	std::filesystem::path SFileDialogs::OpenFile(const char* _Filter)
	{
		// Use Zenity (GTK file dialog) if available
		// Filter format: "*.png *.jpg *.jpeg"
		std::string filter = _Filter ? _Filter : "";
		// Convert filter from Windows format ("Image Files\0*.png;*.jpg\0All Files\0*.*\0") to Zenity format
		// Zenity uses --file-filter="Name | *.ext1 *.ext2"
		std::string zenityFilter = "All files | *";
		if (!filter.empty() && filter != "All Files (*.*)\0*.*\0")
		{
			// Simple: just use the first extension pattern
			std::string ext = filter;
			// Null-separated: take the part after the last null before extension
			auto nullPos = filter.find('\0');
			if (nullPos != std::string::npos)
			{
				ext = filter.substr(nullPos + 1);
				// Remove trailing null
				auto nullEnd = ext.find('\0');
				if (nullEnd != std::string::npos)
					ext = ext.substr(0, nullEnd);
			}
			// Replace semicolons with spaces for Zenity
			for (auto& c : ext) if (c == ';') c = ' ';
			if (!ext.empty())
				zenityFilter = "Selected files | " + ext;
		}

		std::string cmd = "zenity --file-selection --title=\"Open File\"";
		if (zenityFilter != "All files | *")
			cmd += " --file-filter=\"" + zenityFilter + "\"";

		std::string result = ExecCommand(cmd.c_str());
		if (result.empty())
			return std::filesystem::path();

		return std::filesystem::path(result);
	}

	std::filesystem::path SFileDialogs::SaveFile(const char* _Filter)
	{
		std::string filter = _Filter ? _Filter : "";
		std::string zenityFilter = "All files | *";
		if (!filter.empty() && filter != "All Files (*.*)\0*.*\0")
		{
			std::string ext = filter;
			auto nullPos = filter.find('\0');
			if (nullPos != std::string::npos)
			{
				ext = filter.substr(nullPos + 1);
				auto nullEnd = ext.find('\0');
				if (nullEnd != std::string::npos)
					ext = ext.substr(0, nullEnd);
			}
			for (auto& c : ext) if (c == ';') c = ' ';
			if (!ext.empty())
				zenityFilter = "Selected files | " + ext;
		}

		std::string cmd = "zenity --file-selection --save --title=\"Save File\"";
		if (zenityFilter != "All files | *")
			cmd += " --file-filter=\"" + zenityFilter + "\"";

		std::string result = ExecCommand(cmd.c_str());
		if (result.empty())
			return std::filesystem::path();

		return std::filesystem::path(result);
	}
}
