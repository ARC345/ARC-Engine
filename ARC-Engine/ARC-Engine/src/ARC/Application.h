#pragma once
#include "Core.h"

namespace ARC{
	namespace Core{
		class ARC_API CApplication
		{
		public:
			CApplication();
			virtual ~CApplication();

			void Run();
		};

		//Defined in client
		CApplication* CreateApplication();
	}
}