#pragma once

#include "Core.h"

#include "ARC/Renderer/LayerStack.h"

#include <string>
#include <memory>
#include "../Types/vector.h"

namespace ARC { class CLayer; }
namespace ARC { class CImGuiLayer; }

namespace ARC { class CWindowCloseEvent; }
namespace ARC { class CWindowResizeEvent; }
namespace ARC { class CEvent; }

namespace ARC{
	class CWindow;
	class ARC_API CApplication
	{
	public:
		CApplication(const std::string& _Name = "ARC-Engine");
		virtual ~CApplication();

		void Run();
		void OnEvent(CEvent& _e);

		void PushLayer(CLayer* _layer);
		void PushOverlay(CLayer* _overlay);

		inline CWindow& GetWindow() const { return *m_Window; }

		void Shutdown();
		CImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; };

		inline static CApplication& Get() { return *s_Instance; }
		inline virtual std::string GetAppName() =0;
		inline std::string GetEngineName() {return "ARC-Engine";}; 
		inline float GetDeltaTime() const { return m_DeltaTime; }

	private:
		bool OnWindowClose(CWindowCloseEvent& _e);
		bool OnWindowResize(CWindowResizeEvent& _e);
		
	private:
		
		float m_DeltaTime;
		float m_LastFrameTime = 0;
		std::unique_ptr<CWindow> m_Window;
		CImGuiLayer* m_ImGuiLayer;
		uint8_t m_bRunning : 1;
		uint8_t m_bMinimized: 1;
		LayerStack m_LayerStack;
		static CApplication* s_Instance;
	};

	//Defined in client
	CApplication* CreateApplication();
}