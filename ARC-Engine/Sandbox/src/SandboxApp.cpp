#include "ARC.h"
#include <ARC.h>

namespace ARC
{
	class CSandbox : public ARC::Core::CApplication
	{
	public:
		CSandbox() = default;
		~CSandbox() = default;
	};

	Core::CApplication* Core::CreateApplication()
	{
		return new CSandbox;
	}
}