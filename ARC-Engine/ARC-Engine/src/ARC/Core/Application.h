#pragma once

#include "Core.h"

#include "ARC/Layers/LayerStack.h"

#include <string>
#include <memory>

namespace ARC { class CLayer; }
namespace ARC { class CImGuiLayer; }

namespace ARC { class CWindowCloseEvent; }
namespace ARC { class CEvent; }

namespace ARC{
	class CWindow;
	namespace Core{
		class ARC_API CApplication
		{
		public:
			CApplication();
			virtual ~CApplication();

			void Run();
			void OnEvent(CEvent& _e);

			void PushLayer(CLayer* _layer);
			void PushOverlay(CLayer* _overlay);
			
			inline CWindow& GetWindow() const { return *m_Window; }

			inline static CApplication& Get() { return *s_Instance; }
			inline virtual std::string GetAppName() =0; 
			inline class std::string GetEngineName() {return "ARC-Engine";}; 
			inline float GetDeltaTime() const { return m_DeltaTime; }
		private:
			bool OnWindowClose(CWindowCloseEvent& _e);
		
		private:
		
			float m_DeltaTime;
			float m_LastFrameTime;
			std::unique_ptr<CWindow> m_Window;
			CImGuiLayer* m_ImGuiLayer;
			unsigned int m_bRunning : 1;
			LayerStack m_LayerStack;
			static CApplication* s_Instance;
		};

		//Defined in client
		CApplication* CreateApplication();
	}
}