#include "arc_pch.h"

#include <fstream>
#include <streambuf>
#include "ARC\Core\Core.h"
#include "inicpp.h"

namespace ARC {
	static std::unordered_map<std::string, std::shared_ptr<ini::IniFile>> IniFiles;

	namespace HPR {
		std::mt19937 Random::s_RandomEngine;
		std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;
	}

	std::string HPR::IO::ReadFile(const std::string& _Path)
	{
		std::ifstream is(_Path, std::ios::in | std::ios::binary);
		ARC_CORE_ASSERT(is.is_open(), "Could not find the file: {0}", _Path);
		std::string str((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
		is.close();
		return str;
	}

	std::string HPR::IO::ExtractFileNameFromPath(const std::string& _Path, bool _bRemoveExtention /*= true*/)
	{
		// assets/shaders/Texture.glsl
		auto lastSlash = _Path.find_last_of("/\\");
		// Texture.glsl
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		if(!_bRemoveExtention) return _Path.substr(lastSlash);

		auto lastDot = _Path.rfind('.');
		auto count = lastDot == std::string::npos ? _Path.size() - lastSlash : lastDot - lastSlash;

		return _Path.substr(lastSlash, count);
	}
}