#pragma once

namespace ARC
{
	namespace Base
	{
		struct ARC_API FContainer
		{
			FContainer() {};

			virtual const char* GetName() const = 0;
		};
	}
}