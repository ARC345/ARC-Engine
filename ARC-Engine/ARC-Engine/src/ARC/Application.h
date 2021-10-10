#pragma once

#include "Core.h"
#include <string>

namespace ARC{
	namespace Core{
		class ARC_API CApplication
		{
		public:
			CApplication();
			virtual ~CApplication();

			void Run();

			inline virtual std::string GetAppName() =0; 
			inline class std::string GetEngineName() {return "ARC-Engine";}; 
		};

		//Defined in client
		CApplication* CreateApplication();
	}
}