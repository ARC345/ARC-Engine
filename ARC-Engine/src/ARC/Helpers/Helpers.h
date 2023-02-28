#pragma once

#include <random>
#include <limits>
#include <cstdint>
#include <algorithm>
#include <type_traits>
#include "ARC\Types\Vector.h"
#include "ARC\Types\Transform2D.h"
#include "Conversions.h"
#include "Math.h"

#if defined(__GNUC__) || defined(__GNUG__)
	#include <cxxabi.h>
#endif
namespace ARC { namespace Core { class CApplication; } }

namespace ARC { class CEvent; }
#define OUT

struct CounterId {};

enum class EQuadCorner {
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight
};

enum class ERandom {
	Normal,
	None
};

namespace ARC {
	struct SHPR {
		template<typename T, bool TbIncludeNamespace = false>
		static 
		TString GetClassName()
		{
			// @TODO Test code
			#if defined(__GNUC__) || defined(__GNUG__)
				int status;
				std::unique_ptr<char> realname;
				const std::type_info& ti = typeid(e);
				realname.reset(abi::__cxa_demangle(ti.name(), 0, 0, &status));
				assert(status == 0);
				return std::string(realname.get());
			#elif defined(_MSC_VER)
				auto rval = std::string(typeid(T).name());

				std::size_t pos=0;
				if constexpr (TbIncludeNamespace) 
					pos = rval.find_first_of(' ') + 1;
				else 
					pos = rval.find_last_of("::") + 1;

				return rval.substr(pos, rval.size() - pos);
			#else
				ARC_CORE_ASSERT(false, "Undefined for this compiler")
			#endif

		}

		template<typename T, typename U>
		static constexpr 
		size_t GetDataMemberOffset(U T::* member)
		{
			return (char*)&((T*)nullptr->*member) - (char*)nullptr;
		}

		[[nodiscard]] static
		TString ReadFile(const TString& _Path);
		
		[[nodiscard]] static
		TString ExtractFileNameFromPath(const TString& _Path, bool _bRemoveExtention = true);

		template<EQuadCorner T1>
		static
		FVec2 GetScaledCorner(const FTransform2D& pTrans) {
			GetScaledCorner(pTrans, SMath::Sin(pTrans.Rotation), SMath::Cos(pTrans.Rotation));
		}
		template<EQuadCorner T1>
		static
		FVec2 GetScaledCorner(const FTransform2D& pTrans, float pSin, float pCos) {
			switch (T1)
			{
			case EQuadCorner::TopRight: return {
				pTrans.Location.x + ((pTrans.Scale.x / 2) * pCos) - ((pTrans.Scale.y / 2) * pSin),
				pTrans.Location.y + ((pTrans.Scale.x / 2) * pSin) + ((pTrans.Scale.y / 2) * pCos) };
			case EQuadCorner::BottomLeft: return {										    
				pTrans.Location.x - ((pTrans.Scale.x / 2) * pCos) + ((pTrans.Scale.y / 2) * pSin),
				pTrans.Location.y - ((pTrans.Scale.x / 2) * pSin) - ((pTrans.Scale.y / 2) * pCos) };
			case EQuadCorner::TopLeft: return {											    
				pTrans.Location.x - ((pTrans.Scale.x / 2) * pCos) - ((pTrans.Scale.y / 2) * pSin),
				pTrans.Location.y - ((pTrans.Scale.x / 2) * pSin) + ((pTrans.Scale.y / 2) * pCos) };
			case EQuadCorner::BottomRight: return {										    
				pTrans.Location.x + ((pTrans.Scale.x / 2) * pCos) + ((pTrans.Scale.y / 2) * pSin),
				pTrans.Location.y + ((pTrans.Scale.x / 2) * pSin) - ((pTrans.Scale.y / 2) * pCos) };
			}
		}

		template<typename To, typename From>
		static
		To Conv(const From& p) {
			return SConvert<To, From>::Conv(p);
		};

		template<typename T>
		class CustomCounter
		{
			template<int N>
			struct Flag { friend constexpr int setFlag(Flag<N>); };

			template<int N>
			struct Writer
			{
				friend constexpr int setFlag(Flag<N>) { return 0; }
			};

			template<int N>
			static constexpr int reader(float, Flag<N>) { return N; }

			template<int N, int = setFlag(Flag<N>{}) >
			static constexpr int reader(int, Flag<N>, int value = reader(0, Flag<N + 1>{}))
			{
				return value;
			}

		public:
			template<int N = reader(0, Flag<0>{}), int = sizeof(Writer<N>) >
			static constexpr int Next() { return N; }
		};

		struct CounterId_Default : CounterId {};
		using Counter = CustomCounter<CounterId_Default>;
	};
}