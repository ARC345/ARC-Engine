#pragma once

#include "Core.h"

#include "Layers\LayerStack.h"

#include <string>
#include <memory>

namespace ARC { class CWindowCloseEvent; }
namespace ARC { class CEvent; }

namespace ARC{
	class Window;
	namespace Core{
		class ARC_API CApplication
		{
		public:
			CApplication();
			virtual ~CApplication();

			void Run();
			void OnEvent(CEvent& _e);

			void PushLayer(Layer* _layer);
			void PushOverlay(Layer* _overlay);
			
			inline Window& GetWindow() const { return *m_Window; }

			inline static CApplication& Get() { return *s_instance; }
			inline virtual std::string GetAppName() =0; 
			inline class std::string GetEngineName() {return "ARC-Engine";}; 
		private:
			bool OnWindowClose(CWindowCloseEvent& _e);
		
		private:
			std::unique_ptr<Window> m_Window;
			unsigned int m_bRunning : 1;
			LayerStack m_LayerStack;

			static CApplication* s_instance;
		};

		//Defined in client
		CApplication* CreateApplication();
	}
}