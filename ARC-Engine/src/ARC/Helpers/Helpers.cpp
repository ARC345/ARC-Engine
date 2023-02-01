#include "arc_pch.h"
#include "Helpers.h"
#include <fstream>
#include <streambuf>
#include "inicpp.h"

namespace ARC {
	static std::unordered_map<TString, std::shared_ptr<ini::IniFile>> IniFiles;

	TString SHPR::ReadFile(const TString& _Path)
	{
		std::ifstream is(_Path.c_str(), std::ios::in | std::ios::binary);
		ARC_CORE_ASSERT(is.is_open(), "Could not find the file \"{0}\"", _Path.c_str());
		TString str((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
		is.close();
		return str;
	}

	TString SHPR::ExtractFileNameFromPath(const TString& _Path, bool _bRemoveExtention /*= true*/)
	{
		auto lastSlash = _Path.find_last_of("/\\");
			
		lastSlash = lastSlash == TString::npos ? 0 : lastSlash + 1;
		if(!_bRemoveExtention) return _Path.substr(lastSlash);

		auto lastDot = _Path.rfind('.');
		auto count = (lastDot == TString::npos ? _Path.length() - lastSlash : lastDot - lastSlash);

		return _Path.substr(lastSlash, count);
	}
}