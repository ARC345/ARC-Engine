#include "arc_pch.h"
#include "ImGuiLayer.h"
#include "Platform/OpenGl/ImGuiOpenGlRenderer.h"
#include "GLFW/glfw3.h"
#include "../Application.h"
#include "../Window.h"
#include "../Events/Event.h"
#include "../Events/MouseEvent.h"
#include "../Events/KeyEvent.h"
#include "../Events/ApplicationEvent.h"
#include "glad/glad.h"

namespace ARC {
	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}
	ImGuiLayer::~ImGuiLayer() {}
	
	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsClassic();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// TEMPORARY: should eventually use ARC key codes
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDeattach()
	{

	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(ARC::Core::CApplication::Get().GetWindow().GetWidth(), ARC::Core::CApplication::Get().GetWindow().GetHeight());
		
		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
		
		static bool bshow = true;
		ImGui::ShowDemoWindow(&bshow);
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(CEvent& _event)
	{
		ImGuiIO& io = ImGui::GetIO();

		switch (_event.GetEventType())
		{
		case EEventType::EET_MouseButtonPressed: {
			CMouseButtonPressedEvent* x = (CMouseButtonPressedEvent*)&_event;
			io.MouseDown[x->GetMouseButton()] = true;
			break;
			}
		case EEventType::EET_MouseButtonReleased: {
			CMouseButtonReleasedEvent* x = (CMouseButtonReleasedEvent*)&_event;
			io.MouseDown[x->GetMouseButton()] = false;
			break;
			}
		case EEventType::EET_MouseMoved: {
			CMouseMovedEvent* x = (CMouseMovedEvent*)&_event;
			io.MousePos = ImVec2(x->GetX(), x->GetY());
			break;

			}
		case EEventType::EET_MouseScrolled: {
			CMouseScrolledEvent* x = (CMouseScrolledEvent*)&_event;
			io.MouseWheel += x->GetYOffset();
			io.MouseWheelH += x->GetXOffset();
			break;

			}
		case EEventType::EET_KeyPressed: {
			CKeyPressedEvent* x = (CKeyPressedEvent*)&_event;

			if (x->GetKeyCode() >= 0 && x->GetKeyCode() < IM_ARRAYSIZE(io.KeysDown))
				io.KeysDown[x->GetKeyCode()] = true;
			
			// Modifiers are not reliable across systems
			io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
			io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
			io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
#ifdef _WIN32
			io.KeySuper = false;
#else
			io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
#endif
			break;

			}
		case EEventType::EET_KeyReleased: {
			CKeyReleasedEvent* x = (CKeyReleasedEvent*)&_event;

			if (x->GetKeyCode() >= 0 && x->GetKeyCode() < IM_ARRAYSIZE(io.KeysDown))
				io.KeysDown[x->GetKeyCode()]=false;
			break;

			}
		case EEventType::EET_WindowResize: {
			CWindowResizeEvent* x = (CWindowResizeEvent*)&_event;
			io.DisplaySize = ImVec2(x->GetX(), x->GetY());
			io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
			glViewport(0,0, x->GetX(), x->GetY());
			break;
			}
		case EEventType::EET_KeyTyped: {
			CKeyTypedEvent* x = (CKeyTypedEvent*)&_event;
			io.AddInputCharacter(x->GetKeyCode());
			break;
			}
		}
	}
}