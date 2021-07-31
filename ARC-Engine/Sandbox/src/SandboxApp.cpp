#include <ARC.h>

namespace ARC
{
	class CSandbox : public ARC::Core::CApplication
	{
	public:
		CSandbox() = default;
		~CSandbox() = default;

		inline virtual std::string GetAppName() override { return "Sandbox"; };
	};

	Core::CApplication* Core::CreateApplication()
	{
		return new CSandbox;
	}
}