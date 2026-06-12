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
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "ImGuizmo.h"


namespace arc {
	imgui_layer::imgui_layer() : layer("ImGuiLayer"), block_events_(1u) {}
	imgui_layer::~imgui_layer() {}
	
	void imgui_layer::on_attach()
	{
		ARC_PROFILE_FUNCTION();
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsClassic();

		ImGuiIO& io = ImGui::get_io();
		io.IniFilename = "config/imgui.ini";
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskbarIcons;
		
		io.Fonts->AddFontFromFileTTF("assets/fonts/MavenPro-Bold.ttf", 16.f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/MavenPro-Regular.ttf", 16.f);

		ImGuiStyle& style = ImGui::get_style();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding=0.0f;
			style.Colors[ImGuiCol_WindowBg].w=1.0f;
		}

		set_dark_theme_colors();
		ImGui::StyleColorsDark();
		auto& app = application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.get_window().get_native_window());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void imgui_layer::on_detach()
	{
		ARC_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void imgui_layer::on_event(event& _e)
	{
		if (!block_events_) return;
		ImGuiIO& io = ImGui::get_io();
		_e.bHandled |= _e.is_in_category(EventCategoryMouse) & io.WantCaptureMouse;
		_e.bHandled |= _e.is_in_category(EventCategoryKeyboard) & io.WantCaptureKeyboard;
	}

	void imgui_layer::begin()
	{
		ARC_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void imgui_layer::end()
	{
		ARC_PROFILE_FUNCTION();
		ImGuiIO& io = ImGui::get_io();
		io.DisplaySize = ImVec2((float)application::Get().get_window().get_width(), (float)application::Get().get_window().get_height());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::get_draw_data());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void imgui_layer::set_dark_theme_colors()
	{
		auto& colors = ImGui::get_style().Colors;
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

	void imgui_layer::on_gui_render()
	{
		//static bool show = true;
		//ImGui::ShowDemoWindow(&show);
	}
}