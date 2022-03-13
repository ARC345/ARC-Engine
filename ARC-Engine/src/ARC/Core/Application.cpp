#include "arc_pch.h"
#include "Application.h"
#include "ARC/Types/vector.h"
#include "ARC/Events/ApplicationEvent.h"
#include "Window.h"
#include "glad\glad.h"
#include "ARC/Input\Input.h"
#include "ARC/Renderer/Shader.h"
#include "ARC/Renderer/Buffer.h"
#include "ARC/Renderer/VertexArray.h"
#include "ARC/Renderer/Renderer.h"
#include "ARC/GUI/ImGuiLayer.h"
#include "GLFW\glfw3.h"
#include "Macros.h"
#include "ARC/Profiling/Timer.h"
#include "../Helpers/Math.h"

namespace ARC
{
	namespace Core
	{
		CApplication* CApplication::s_Instance=nullptr;
		CApplication::CApplication(const std::string& _Name /*= "ARC-Engine"*/) :
			m_bRunning(1u)
		{
			ARC_PROFILE_FUNCTION();

			ARC_CORE_ASSERT(!s_Instance, "Application already exists")
			ARC_CORE_INFO("{0}", &CApplication::OnEvent);
			ARC_CORE_INFO("{0}", ARC::Math::Conv<long double, Day, Minute>(1));

			s_Instance= this;

			m_Window = std::unique_ptr<CWindow>(CWindow::Create(_Name));
			
			CRenderer::Init();

			m_ImGuiLayer = new CImGuiLayer();
			PushOverlay(m_ImGuiLayer);
			
			m_Window->SetEventCallback(BIND_FN(&CApplication::OnEvent));
		}

		CApplication::~CApplication() {}

		void CApplication::Run()
		{
			ARC_PROFILE_FUNCTION();
			while (m_bRunning)
			{
				//@TEMP
				float time = (float)glfwGetTime();
				m_DeltaTime = time - m_LastFrameTime;
				m_LastFrameTime = time;

				if(!m_bMinimized) {
					for (CLayer* layer : m_LayerStack)
						layer->OnUpdate(m_DeltaTime);
				}
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

		void CApplication::Shutdown()
		{
			m_bRunning=false;
		}

		bool CApplication::OnWindowClose(CWindowCloseEvent& _e)
		{
			m_bRunning = false;
			return true;
		}
		bool CApplication::OnWindowResize(CWindowResizeEvent& _e)
		{
			if(_e.GetX() == 0 || _e.GetY() == 0){
				m_bMinimized = true;
				return false;
			}
			m_bMinimized = false;
			GetWindow().SetWidth(_e.GetX());
			GetWindow().SetHeight(_e.GetY());
			CRenderer::OnWindowResize(TVec2<uint32_t>(_e.GetX(), _e.GetY()));
			return false;
		}
	}
}