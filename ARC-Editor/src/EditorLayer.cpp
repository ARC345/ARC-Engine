#include "PCH/arc_pch.h"
#include "EditorLayer.h"
#include "imgui/imgui.h"
#include "ARC/Objects/Primitive2D.h"
#include "ARC/Types/Color.h"
#include "ARC/Renderer/Renderer2D.h"
#include "ARC/Renderer/RenderCommand.h"
#include "ARC/Scene/Scene.h"
#include "ARC/Scene/Entity.h"
#include "ARC/Types/Glm.h"
#include "glm/ext/matrix_clip_space.inl"
#include "ARC/Core/PlatformUtils.h"
#include "ARC/Renderer/FrameBuffer.h"
#include "Atom/Atom.h"
#include "ImGuizmo.h"

namespace ARC {
	static CEntity ESquare;
	static CEntity ECamera;

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

		m_ActiveScene = CreateRef<CScene>();
		m_AtomExp = CreateRef<CAtomExp>();
		m_SceneHierachyPanel.SetContext(m_ActiveScene);
		
		m_AtomExp->OnAttach(m_ActiveScene);
	}

	void CEditorLayer::OnDetach()
	{
		m_AtomExp->OnDetach();
	}

	void CEditorLayer::OnUpdate(float _DeltaTime)
	{
		if (SFrameBufferSpecifications spec = m_FrameBuffer->GetSpecifications();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_FrameBuffer->Resize({ (uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y });
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

			m_ActiveScene->OnViewportResize({ (uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y });
		}

		if (m_ViewportHovered && m_ViewportFocused)	m_CameraController.OnUpdate(_DeltaTime);
		
		CRenderer2D::ResetStats();

		m_FrameBuffer->Bind();

		CRenderCommand::SetClearColour({ .1f, .1f, .1f, 1.f });
		CRenderCommand::Clear();

		m_ActiveScene->OnUpdate(_DeltaTime);
		m_AtomExp->OnUpdate(_DeltaTime);
		m_FrameBuffer->UnBind();
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

					if (ImGui::MenuItem("New")) {}
					if (ImGui::MenuItem("Save")) {}
					if (ImGui::MenuItem("Open...", "Ctrl+O"))
					{
						TString filepath = CFileDialogs::OpenFile("ARC-Engine Scene (*.arc)\0*.arc\0");
						if (!filepath.empty())
						{
							m_ActiveScene = CreateRef<CScene>();
							m_ActiveScene->DeserializeFromText(filepath);

							m_ActiveScene->OnViewportResize({ (uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y });
							m_SceneHierachyPanel.SetContext(m_ActiveScene);
						}
					}
					if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					{
						auto filepath = CFileDialogs::SaveFile("ARC-Engine Scene (*.arc)\0*.arc\0");
						if (!filepath.empty())
							m_ActiveScene->SerializeToText(filepath);
					}
					if (ImGui::MenuItem("Exit")) CApplication::Get().Shutdown();
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			m_SceneHierachyPanel.OnImGuiRender();

			ImGui::Begin("Settings");

			auto stats = CRenderer2D::GetStats();
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
			CApplication::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused || !m_ViewportHovered);
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 1, 0 }, ImVec2{ 0, 1 });
			
			// Gizmos
			const CEntity selectedEntity = m_SceneHierachyPanel.GetSelectedEntity();
			if (selectedEntity)
			{
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();
				//ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);


			}

			ImGui::End();
			ImGui::PopStyleVar();

			m_AtomExp->OnGuiRender();
			ImGui::End();

		}
	}
}