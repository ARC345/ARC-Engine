#include "arc_pch.h"
#include "Application.h"
#include "ARC/Types/vector.h"
#include "ARC/Events/ApplicationEvent.h"
#include "Window.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "ARC/Input\Input.h"
#include "ARC/Renderer/Shader.h"
#include "ARC/Renderer/Buffer.h"
#include "ARC/Renderer/VertexArray.h"
#include "ARC/Renderer/Renderer.h"
#include "ARC/GUI/ImGuiLayer.h"
#include "Macros.h"
#include "ARC/Profiling/Timer.h"
#include "ARC/Helpers/Math.h"

namespace ARC
{
	CApplication* CApplication::s_Instance=nullptr;
	CApplication::CApplication(const TString& _Name /*= "ARC-Engine"*/) :
		mbRunning(1u)
	{
		ARC_PROFILE_FUNCTION();

		ARC_CORE_ASSERT(!s_Instance, "Application already exists")
		ARC_CORE_INFO("{0}", &CApplication::OnEvent);
		ARC_CORE_INFO("{0}", ARC::SMath::Conv<long double, ETimeType::Day, ETimeType::Minute>(1));
			
		ARC_CORE_INFO("Counter: {0}", SHPR::Counter::Next());
		s_Instance= this;

		mWindow = std::unique_ptr<CWindow>(CWindow::Create(_Name));
			
		CRenderer::Init();

		mImGuiLayer = new CImGuiLayer();
		PushOverlay(mImGuiLayer);
			
		mWindow->SetEventCallback(BIND_FN(&CApplication::OnEvent));
	}

	CApplication::~CApplication() {}

	void CApplication::Run()
	{
		ARC_PROFILE_FUNCTION();
		while (mbRunning)
		{
			//@TEMP
			auto time = (float)glfwGetTime();
			mDeltaTime = time - mLastFrameTime;
			mLastFrameTime = time;

			if(!mbMinimized) {
				for (CLayer* layer : mLayerStack)
					layer->OnUpdate(mDeltaTime);
			}
			mImGuiLayer->Begin();
			for (CLayer* layer : mLayerStack)
				layer->OnGuiRender();
			mImGuiLayer->End();
			mWindow->OnUpdate();
		}
	}

	void CApplication::OnEvent(CEvent& _e)
	{
		CEventDispatcher dispatcher(_e);
		dispatcher.Dispatch<CWindowCloseEvent>(BIND_FN(&CApplication::OnWindowClose));
		dispatcher.Dispatch<CWindowResizeEvent>(BIND_FN(&CApplication::OnWindowResize));

		for (auto it = mLayerStack.end(); it!= mLayerStack.begin();)
		{
			(*--it)->OnEvent(_e);
			if (_e.bHandled) break;
		}
	}

	void CApplication::PushLayer(CLayer* _layer)
	{
		mLayerStack.PushLayer(_layer);
		_layer->OnAttach();
	}
	void CApplication::PushOverlay(CLayer* _overlay)
	{
		mLayerStack.PushOverlay(_overlay);
		_overlay->OnAttach();
	}

	void CApplication::Shutdown()
	{
		mbRunning=false;
	}

	bool CApplication::OnWindowClose(CWindowCloseEvent& _e)
	{
		mbRunning = false;
		return true;
	}
	bool CApplication::OnWindowResize(CWindowResizeEvent& _e)
	{
		if(_e.GetX() == 0 || _e.GetY() == 0){
			mbMinimized = true;
			return false;
		}
		mbMinimized = false;
		GetWindow().SetWidth(_e.GetX());
		GetWindow().SetHeight(_e.GetY());
		CRenderer::OnWindowResize(TVec2<uint32_t>(_e.GetX(), _e.GetY()));
		return false;
	}
}