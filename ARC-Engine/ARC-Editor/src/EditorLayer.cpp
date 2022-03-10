#include "PCH\arc_pch.h"
#include "EditorLayer.h"
#include "imgui\imgui.h"
#include "ARC\Objects\Primitive2D.h"
#include "ARC\Renderer\Color.h"
#include "ARC\Renderer\Renderer2D.h"
#include "ARC\Renderer\RenderCommand.h"

namespace ARC {
	CEditorLayer::CEditorLayer() :
		CLayer("EditorLayer"),
		m_CameraController(1280.f / 780.f, true)
	{
	}

	void CEditorLayer::OnAttach()
	{
		SFrameBufferSpecifications frame_buffer_specs;
		frame_buffer_specs.Width = 1280;
		frame_buffer_specs.Height = 720;
		m_FrameBuffer = CFrameBuffer::Create(frame_buffer_specs);
		m_ViewportSize = { 1280.f, 720.f };
	}

	void CEditorLayer::OnDetach()
	{
	}

	void CEditorLayer::OnUpdate(float _DeltaTime)
	{
		// test
		ARC::CRenderer2D::ResetStats();
		if (m_ViewportHovered && m_ViewportFocused)
		{
			m_CameraController.OnUpdate(_DeltaTime);
		}

		m_FrameBuffer->Bind();
		CRenderCommand::SetClearColour({ .1f, .1f, .1f, 1.f });
		CRenderCommand::Clear();

		CRenderer2D::BeginScene(m_CameraController.GetCamera());

		CPrimitive2D Quad2;
		Quad2.Transform.Location = { 0.f, 0.f };
		Quad2.Transform.Rotation = 0;
		Quad2.Transform.Scale = { 1.f, 1.f };
		Quad2.Transform.ZOrder = 0.2f;
		Quad2.Color = CColor::Red;

		CRenderer2D::DrawQuad(Quad2);

		CRenderer2D::EndScene();
		m_FrameBuffer->UnBind();
		m_CameraController.GetCamera().RecalculateViewProjectionMatrix();
	}

	void CEditorLayer::OnGuiRender()
	{
		ARC_PROFILE_FUNCTION();
		// Note: Switch this to true to enable dockspace
		static bool dockingEnabled = true;
		if (dockingEnabled)
		{
			static bool dockspaceOpen = true;
			static bool opt_fullscreen_persistant = true;
			bool opt_fullscreen = opt_fullscreen_persistant;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen)
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->Pos);
				ImGui::SetNextWindowSize(viewport->Size);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}

			// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
			// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
			// all active windows docked into it will lose their parent and become undocked.
			// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
			// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
			ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			// DockSpace
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					// Disabling fullscreen would allow the window to be moved to the front of other windows, 
					// which we can't undo at the moment without finer window depth/z control.

					if (ImGui::MenuItem("Exit")) Core::CApplication::Get().Shutdown();
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}
			ImGui::Begin("Settings");

			auto stats = ARC::CRenderer2D::GetStats();
			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

			ImGui::End();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			ImGui::Begin("Viewport");
			//@todo
			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			Core::CApplication::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused || !m_ViewportHovered);
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			if (m_ViewportSize != *((FVec2*)&viewportPanelSize))
			{
				m_FrameBuffer->Resize({(uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y});
				m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

				m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
			}
			uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 1, 0 }, ImVec2{ 0, 1 });
			ImGui::End();
			ImGui::PopStyleVar();

			ImGui::End();
		}
	}
}