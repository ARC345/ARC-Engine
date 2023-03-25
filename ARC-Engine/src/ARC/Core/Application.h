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
		CApplication(const TString& _Name = "ARC-Engine");
		virtual ~CApplication();

		void Run();
		void OnEvent(CEvent& _e);

		void PushLayer(CLayer* _layer);
		void PushOverlay(CLayer* _overlay);

		inline CWindow& GetWindow() const { return *mWindow; }

		void Shutdown();
		CImGuiLayer* GetImGuiLayer() { return mImGuiLayer; };

		inline static CApplication& Get() { return *s_Instance; }
		inline virtual TString GetAppName() =0;
		inline const TString& GetEngineName() { static TString rval = "ARC_Engine"; return rval; };
		inline float GetDeltaTime() const { return mDeltaTime; }

	private:
		bool OnWindowClose(CWindowCloseEvent& pE);
		bool OnWindowResize(CWindowResizeEvent& pE);
		
	private:
		
		float mDeltaTime;
		float mLastFrameTime = 0;
		
		std::unique_ptr<CWindow> mWindow;
		
		CImGuiLayer* mImGuiLayer;
		TUInt8 mbRunning : 1;
		TUInt8 mbMinimized: 1;
		LayerStack mLayerStack;
		static CApplication* s_Instance;
	};

	//Defined in client
	CApplication* CreateApplication();
}