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
			virtual __forceinline const size_t& GetSize() const = 0;
		};
	}
}