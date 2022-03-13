#pragma once
#include <string>
#include "ARC\Core\Macros.h"

#define ARC_CLASS(x)\
		public:\
			static inline std::string GetName() { return std::string(#x); }\
		private:
namespace ARC
{
	namespace Base
	{
		struct ARC_API TTypeBase
		{
		public:
			TTypeBase() {};
			static inline std::string GetName() { return "TTypeBase"; }
		};
	}
}