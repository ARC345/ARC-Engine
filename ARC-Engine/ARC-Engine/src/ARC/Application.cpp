#include "arc_pch.h"
#include "Application.h"
#include "Types\vector.h"
#include "Log.h"
#include "Events\ApplicationEvent.h"

namespace ARC
{
	namespace Core
	{
		CApplication::CApplication()
		{

		}

		CApplication::~CApplication()
		{

		}

		void CApplication::Run()
		{
			CWindowResizeEvent e(1280, 720);
			ARC_TRACE(e.ToString());

			while (1)
			{
			}
		}	
	}
}