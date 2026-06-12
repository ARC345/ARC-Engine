#include "arc_pch.h"
#include "Helpers.h"
#include <fstream>
#include <streambuf>
#include "inicpp.h"

namespace arc {
	static std::unordered_map<string, std::shared_ptr<ini::IniFile>> IniFiles;

	string shpr::ReadFile(const string& path)
	{
		std::ifstream is(path.c_str(), std::ios::in | std::ios::binary);
		ARC_CORE_ASSERT(is.is_open(), "Could not find the file \"{0}\"", path.c_str());
		string str((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
		is.close();
		return str;
	}

	string shpr::ExtractFileNameFromPath(const string& path, bool _bRemoveExtention /*= true*/)
	{
		auto lastSlash = path.find_last_of("/\\");
			
		lastSlash = lastSlash == string::npos ? 0 : lastSlash + 1;
		if(!_bRemoveExtention) return path.substr(lastSlash);

		auto lastDot = path.rfind('.');
		auto count = (lastDot == string::npos ? path.length() - lastSlash : lastDot - lastSlash);

		return path.substr(lastSlash, count);
	}
}