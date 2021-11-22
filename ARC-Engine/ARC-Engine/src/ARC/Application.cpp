#include "arc_pch.h"
#include "Application.h"
#include "Types\vector.h"
#include "Log.h"
#include "Events\ApplicationEvent.h"
#include "Window.h"
#include "glad\glad.h"
#include "Input\Input.h"

namespace ARC
{
	namespace Core
	{
		CApplication* CApplication::s_instance=nullptr;
		
		CApplication::CApplication() : m_bRunning(1u)
		{
			ARC_CORE_ASSERT(!s_instance, "Application already exists")
			s_instance= this;
			m_Window = std::unique_ptr<CWindow>(CWindow::Create());
			m_ImGuiLayer = new CImGuiLayer();
			PushOverlay(m_ImGuiLayer);
			m_Window->SetEventCallback(BIND_FUNC(&CApplication::OnEvent));
		}

		CApplication::~CApplication() {}

		void CApplication::Run()
		{
			while (m_bRunning)
			{
				glClearColor(1, 0, 1, 1);
				glClear(GL_COLOR_BUFFER_BIT);

				for (CLayer* layer : m_LayerStack)
					layer->OnUpdate();
					
				m_ImGuiLayer->Begin();
				for (CLayer* layer : m_LayerStack)
					layer->OnGuiRender();
				m_ImGuiLayer->End();
				m_Window->OnUpdate();
			}
		}

		void CApplication::OnEvent(CEvent& _e)
		{
			CEventDispatcher dispatcher(_e);
			dispatcher.Dispatch<CWindowCloseEvent>(BIND_FUNC(&CApplication::OnWindowClose));

			for (auto it = m_LayerStack.end(); it!= m_LayerStack.begin();)
			{
				(*--it)->OnEvent(_e);
				if (_e.bHandled) break;
			}
		}

		void CApplication::PushLayer(CLayer* _layer)
		{
			m_LayerStack.PushLayer(_layer);
			_layer->OnAttach();
		}
		void CApplication::PushOverlay(CLayer* _overlay)
		{
			m_LayerStack.PushOverlay(_overlay);
			_overlay->OnAttach();
		}
		bool CApplication::OnWindowClose(CWindowCloseEvent& _e)
		{
			m_bRunning = false;
			return true;
		}
	}
}