#include "arc_pch.h"
#include "ImGuiLayer.h"
#include "GLFW/glfw3.h"
#include "ARC/Core/Application.h"
#include "ARC/Core/Window.h"
#include "ARC/Events/Event.h"
#include "ARC/Events/MouseEvent.h"
#include "ARC/Events/KeyEvent.h"
#include "ARC/Events/ApplicationEvent.h"
#include "glad/glad.h"

#define IMGUI_IMPL_API
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "../Helpers/Helpers.h"

namespace ARC {
	CImGuiLayer::CImGuiLayer() : CLayer("ImGuiLayer"), m_bBlockEvents(1u) {}
	CImGuiLayer::~CImGuiLayer() {}
	
	void CImGuiLayer::OnAttach()
	{
		ARC_PROFILE_FUNCTION();
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsClassic();

		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = "config/imgui.ini";
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskbarIcons;
		
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding=0.0f;
			style.Colors[ImGuiCol_WindowBg].w=1.0f;
		}

		auto& app = ARC::Core::CApplication::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void CImGuiLayer::OnDetach()
	{
		ARC_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void CImGuiLayer::OnEvent(CEvent& _e)
	{
		if (!m_bBlockEvents) return;
		ImGuiIO& io = ImGui::GetIO();
		_e.bHandled |= _e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
		_e.bHandled |= _e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
	}

	void CImGuiLayer::Begin()
	{
		ARC_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void CImGuiLayer::End()
	{
		ARC_PROFILE_FUNCTION();
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)ARC::Core::CApplication::Get().GetWindow().GetWidth(), (float)ARC::Core::CApplication::Get().GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void CImGuiLayer::OnGuiRender()
	{
		//static bool show = true;
		//ImGui::ShowDemoWindow(&show);
	}
}