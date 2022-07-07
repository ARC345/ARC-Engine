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

#include "ImGuizmo.h"

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
		
		io.Fonts->AddFontFromFileTTF("assets/fonts/MavenPro-Bold.ttf", 16.f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/MavenPro-Regular.ttf", 16.f);

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding=0.0f;
			style.Colors[ImGuiCol_WindowBg].w=1.0f;
		}

		SetDarkThemeColors();
		ImGui::StyleColorsDark();
		auto& app = ARC::CApplication::Get();
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
		ImGuizmo::BeginFrame();
	}

	void CImGuiLayer::End()
	{
		ARC_PROFILE_FUNCTION();
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)ARC::CApplication::Get().GetWindow().GetWidth(), (float)ARC::CApplication::Get().GetWindow().GetHeight());

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

	void CImGuiLayer::SetDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;
// 		colors[ImGuiCol_WindowBg]				= ImVec4(.01, .012, .031, 1);
// 		colors[ImGuiCol_PopupBg]				= ImVec4(0.2, 0.2, 0.2, 1);
// 		colors[ImGuiCol_Border]					= ImVec4(1, 1, 1, 0.8);
// 		colors[ImGuiCol_FrameBg]				= ImVec4(1, 0, 0, 1);
// 		colors[ImGuiCol_FrameBgHovered]			= ImVec4(.3, .3, .3, 1);
// 		colors[ImGuiCol_FrameBgActive]			= ImVec4(.4, .4, .4, 1);
// 		colors[ImGuiCol_TitleBg]				= ImVec4(0.31, 0.34, 0.38, 1);
// 		colors[ImGuiCol_TitleBgActive]			= ImVec4(0.41, 0.44, 0.48, 1);
// 		colors[ImGuiCol_MenuBarBg]				= ImVec4(0.1, 0.1, 0.1, 1);
// 		colors[ImGuiCol_ScrollbarBg]			= ImVec4(0, 0, 0, 1);
// 		colors[ImGuiCol_ScrollbarGrab]			= ImVec4(0.8, .852, .81, 1);
// 		colors[ImGuiCol_ScrollbarGrabHovered]	= ImVec4(0.81, .852, .81, 1);
// 		colors[ImGuiCol_ScrollbarGrabActive]	= ImVec4(0.64, .692, .641, 1);
// 		colors[ImGuiCol_Header]					= ImVec4(0.31, 0.34, 0.38, 1);
// 		colors[ImGuiCol_HeaderHovered]			= ImVec4(1, .212, .231, 1);
// 		colors[ImGuiCol_HeaderActive]			= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_Separator]				= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_SeparatorHovered]		= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_SeparatorActive]		= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_ResizeGrip]				= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_ResizeGripHovered]		= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_ResizeGripActive]		= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_Tab]					= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_TabHovered]				= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_TabActive]				= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_TabUnfocused]			= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_TabUnfocusedActive]		= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_DockingPreview]			= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_DockingEmptyBg]			= ImVec4(1, .212, .231, 1);
// 		colors[ImGuiCol_PlotLines]				= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_PlotLinesHovered]		= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_PlotHistogram]			= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_PlotHistogramHovered]	= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_TableHeaderBg]			= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_TableBorderStrong]		= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_TableBorderLight]		= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_TableRowBg]				= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_TableRowBgAlt]			= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_TextSelectedBg]			= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_DragDropTarget]			= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_NavHighlight]			= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_NavWindowingHighlight]	= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_NavWindowingDimBg]		= ImVec4(.165, .212, .231, 1);
// 		colors[ImGuiCol_ModalWindowDimBg]		= ImVec4(.165, .212, .231, 1);
	}

	void CImGuiLayer::OnGuiRender()
	{
		//static bool show = true;
		//ImGui::ShowDemoWindow(&show);
	}
}