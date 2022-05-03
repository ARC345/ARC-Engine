#include "arc_pch.h"
#include "Helpers.h"
#include <fstream>
#include <streambuf>
#include "ARC\Core\Core.h"
#include "inicpp.h"
#include "ARC\Types\Transform2D.h"
#include "ARC\Types\Glm.h"
#include "glm\gtx\transform.hpp"

namespace ARC {
	static std::unordered_map<std::string, std::shared_ptr<ini::IniFile>> IniFiles;

	namespace HPR {
		std::mt19937 Random::s_RandomEngine32;
		std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;

		template<typename To, typename From>
		To Conv(const From& _)
		{
			static_assert(false, "Conversion not defined");
		}
		template<>
		FGMat4 Conv(const FTransform2D& _) 
		{
			return glm::translate(FGMat4(1.0f), FGVec3(_.Location.x, _.Location.y, _.ZOrder)) *
				glm::rotate(FGMat4(1.0f), _.Rotation, FGVec3(0, 0, 1)) *
				glm::scale(FGMat4(1.0f), FGVec3(_.Scale.x, _.Scale.y, 1.0f));
		}

		std::string IO::ReadFile(const std::string& _Path)
		{
			std::ifstream is(_Path, std::ios::in | std::ios::binary);
			ARC_CORE_ASSERT(is.is_open(), "Could not find the file \"{0}\"", _Path.c_str());
			std::string str((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
			is.close();
			return str;
		}

		std::string IO::ExtractFileNameFromPath(const std::string& _Path, bool _bRemoveExtention /*= true*/)
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
}