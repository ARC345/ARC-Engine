#pragma once
#include <string>

#define ARC_CLASS(x)\
		public:\
			static inline std::string GetName() { return std::string(#x); }\
		private:
namespace ARC
{
	namespace Base
	{
		class ARC_API CObjectBase {
			ARC_CLASS(CObjectBase);
		public:
			CObjectBase(){};
		};
		struct ARC_API TTypeBase
		{
		public:
			TTypeBase() {};
			static inline std::string GetName() { return "TTypeBase"; }
		};
	}
}