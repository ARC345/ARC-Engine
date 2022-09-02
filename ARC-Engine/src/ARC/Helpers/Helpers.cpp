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
	static std::unordered_map<TString, std::shared_ptr<ini::IniFile>> IniFiles;

	namespace HPR {
		std::mt19937 Random::s_RandomEngine32;
		std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;

		template<typename To, typename From>
		To Conv(const From& _)
		{
			return (To)_;
		}
		template<>
		FGMat4 Conv(const FTransform2D& _) 
		{
			return glm::translate(FGMat4(1.0f), FGVec3(_.Location.x, _.Location.y, _.Location.z)) *
				glm::rotate(FGMat4(1.0f), _.Rotation, FGVec3(0, 0, 1)) *
				glm::scale(FGMat4(1.0f), FGVec3(_.Scale.x, _.Scale.y, 1.0f));
		}
		template<typename To, typename From>
		TVec<3, To> Conv(const TVec<3, From>& _)
		{
			return TVec<3, To>(_[0], _[1], _[2]);
		}
		TString IO::ReadFile(const TString& _Path)
		{
			std::ifstream is(_Path, std::ios::in | std::ios::binary);
			ARC_CORE_ASSERT(is.is_open(), "Could not find the file \"{0}\"", _Path.c_str());
			TString str((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
			is.close();
			return str;
		}

		TString IO::ExtractFileNameFromPath(const TString& _Path, bool _bRemoveExtention /*= true*/)
		{
			auto lastSlash = _Path.find_last_of("/\\");
			
			lastSlash = lastSlash == TString::NPos() ? 0 : lastSlash + 1;
			if(!_bRemoveExtention) return _Path.substr(lastSlash);

			auto lastDot = _Path.rfind('.');
			auto count = lastDot == TString::NPos() ? _Path.size() - lastSlash : lastDot - lastSlash;

			return _Path.substr(lastSlash, count);
		}
	}
}