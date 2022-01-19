#include "arc_pch.h"
#include "Application.h"
#include "ARC/Types/vector.h"
#include "ARC/Events/ApplicationEvent.h"
#include "Window.h"
#include "glad\glad.h"
#include "ARC/Input\Input.h"
#include "ARC/Renderer/Shader.h"
#include "ARC/Buffer/Buffer.h"
#include "ARC/Renderer/VertexArray.h"
#include "ARC/Renderer/Renderer.h"
#include "ARC/GUI/ImGuiLayer.h"
#include "GLFW\glfw3.h"

namespace ARC
{
	namespace Core
	{
		CApplication* CApplication::s_Instance=nullptr;
		
		CApplication::CApplication() :
			m_bRunning(1u)
		{
			ARC_CORE_ASSERT(!s_Instance, "Application already exists")
			ARC_CORE_INFO("{0}", &CApplication::OnEvent);

			s_Instance= this;

			m_Window = std::unique_ptr<CWindow>(CWindow::Create());
			
			CRenderer::Init();

			m_ImGuiLayer = new CImGuiLayer();
			PushOverlay(m_ImGuiLayer);
			
			m_Window->SetEventCallback(BIND_FN(&CApplication::OnEvent));
		}

		CApplication::~CApplication() {}

		void CApplication::Run()
		{
			while (m_bRunning)
			{
				//@TEMP
				float time = (float)glfwGetTime();
				m_DeltaTime = time - m_LastFrameTime;
				m_LastFrameTime = time;
				for (CLayer* layer : m_LayerStack)
					layer->OnUpdate(m_DeltaTime);
					
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
			dispatcher.Dispatch<CWindowCloseEvent>(BIND_FN(&CApplication::OnWindowClose));
			dispatcher.Dispatch<CWindowResizeEvent>(BIND_FN(&CApplication::OnWindowResize));

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
		bool CApplication::OnWindowResize(CWindowResizeEvent& _e)
		{
			if(_e.GetDimentions() == {0, 0})
			return false;
		}
	}
}