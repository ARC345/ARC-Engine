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
namespace arc { namespace Core { class application; } }

namespace arc { class event; }
#define OUT

struct CounterId {};

enum class quad_corner {
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight
};

namespace arc {
	struct shpr {
		template<typename T, bool TbIncludeNamespace = false>
		static 
		string get_class_name()
		{
			// @TODO Test code
			#if defined(__GNUC__) || defined(__GNUG__)
				int status;
				std::unique_ptr<char> realname;
				const std::type_info& ti = typeid(e);
				realname.reset(abi::__cxa_demangle(ti.name(), 0, 0, &status));
				assert(status == 0);
				return std::string(realname.get());
			#elif defined(msc_ver)
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
		size_t get_data_member_offset(U T::* member)
		{
			return (char*)&((T*)nullptr->*member) - (char*)nullptr;
		}

		[[nodiscard]] static
		string ReadFile(const string& path);
		
		[[nodiscard]] static
		string ExtractFileNameFromPath(const string& path, bool _bRemoveExtention = true);

		template<quad_corner T1>
		static
		vec2 get_scaled_corner(const transform_2d& pTrans) {
			get_scaled_corner(pTrans, math::Sin(pTrans.Rotation), math::Cos(pTrans.Rotation));
		}
		template<quad_corner T1>
		static
		vec2 get_scaled_corner(const transform_2d& pTrans, float pSin, float pCos) {
			switch (T1)
			{
			case quad_corner::TopRight: return {
				pTrans.Location.x + ((pTrans.Scale.x / 2) * pCos) - ((pTrans.Scale.y / 2) * pSin),
				pTrans.Location.y + ((pTrans.Scale.x / 2) * pSin) + ((pTrans.Scale.y / 2) * pCos) };
			case quad_corner::BottomLeft: return {										    
				pTrans.Location.x - ((pTrans.Scale.x / 2) * pCos) + ((pTrans.Scale.y / 2) * pSin),
				pTrans.Location.y - ((pTrans.Scale.x / 2) * pSin) - ((pTrans.Scale.y / 2) * pCos) };
			case quad_corner::TopLeft: return {											    
				pTrans.Location.x - ((pTrans.Scale.x / 2) * pCos) - ((pTrans.Scale.y / 2) * pSin),
				pTrans.Location.y - ((pTrans.Scale.x / 2) * pSin) + ((pTrans.Scale.y / 2) * pCos) };
			case quad_corner::BottomRight: return {										    
				pTrans.Location.x + ((pTrans.Scale.x / 2) * pCos) + ((pTrans.Scale.y / 2) * pSin),
				pTrans.Location.y + ((pTrans.Scale.x / 2) * pSin) - ((pTrans.Scale.y / 2) * pCos) };
			}
		}

		template<typename To, typename From>
		static
		To Conv(const From& p) {
			return convert<To, From>::Conv(p);
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