#pragma once

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned long ulong;

namespace ARC
{
	namespace Base
	{
		struct FContainer
		{
			FContainer() {};
		protected:
			virtual constexpr [[nodiscard]] size_t GetSize() const = 0;
		};
	}
}