#include "PCH/arc_pch.h"
#include "EditorLayer.h"
#include "imgui/imgui.h"
#include "ARC/Objects/Primitive2D.h"
#include "ARC/Renderer/Renderer2D.h"
#include "ARC/Renderer/RenderCommand.h"
#include "ARC/Scene/Scene.h"
#include "ARC/Scene/Entity.h"
#include "ARC/Renderer/FrameBuffer.h"
#include "LifeSim2D/LifeSim2D.h"
#include "ImGuizmo.h"
#include "glm/gtc/type_ptr.inl"
#include "glm\gtx\transform.hpp"
#include "ARC/Wrappers/Glm.h"
#include "ARC/Scene/BasicComponents.h"
#include "ARC/Events/MouseEvent.h"

namespace ARC {
	static CEntity ESquare;
	static CEntity ECamera;

	CEditorLayer::CEditorLayer() :
		CLayer("EditorLayer"),
		mCameraController(1280.f / 780.f, true)
	{
	}

	void CEditorLayer::OnAttach()
	{
		SFrameBufferSpecification frame_buffer_specs;
		frame_buffer_specs.Width = 1280;
		frame_buffer_specs.Height = 720;
		frame_buffer_specs.Attachments = { EFrameBufferTextureFormat::RGBA8, EFrameBufferTextureFormat::RED_INTEGER, EFrameBufferTextureFormat::DEPTH24STENCIL8 };
		mFrameBuffer = CFrameBuffer::Create(frame_buffer_specs);

		mActiveScene = CreateRef<CScene>();
		mLifeSim2D = CreateRef<CLifeSim2D>();
		mSceneHierachyPanel.SetContext(mActiveScene);
		mEditorCamera = CEditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		mLifeSim2D->OnAttach(mActiveScene);
	}

	void CEditorLayer::OnDetach()
	{
		mLifeSim2D->OnDetach();
	}

	void CEditorLayer::OnUpdate(float _DeltaTime)
	{
		if (SFrameBufferSpecification spec = mFrameBuffer->GetSpecifications();
			mViewportSize.x > 0.0f && mViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != mViewportSize.x || spec.Height != mViewportSize.y))
		{
			mFrameBuffer->Resize({ (TUInt32)mViewportSize.x, (TUInt32)mViewportSize.y });
			mCameraController.OnResize(mViewportSize.x, mViewportSize.y);
			mEditorCamera.SetViewportSize(mViewportSize.x, mViewportSize.y);
			mActiveScene->OnViewportResize({ (TUInt32)mViewportSize.x, (TUInt32)mViewportSize.y });
		}

		if (mViewportHovered && mViewportFocused)	mCameraController.OnUpdate(_DeltaTime);
		
		CRenderer2D::ResetStats();

		mFrameBuffer->Bind();

		CRenderCommand::SetClearColour({ .1f, .1f, .1f, 1.f });
		CRenderCommand::Clear();

		mFrameBuffer->ClearColorAttachment(1, -1);

		mEditorCamera.OnUpdate(_DeltaTime);
		mActiveScene->OnUpdateEditor(_DeltaTime, mEditorCamera);

		auto viewportSize = mViewportMaxBound-mViewportMinBound;
		auto MousePos = (FVec2&)ImGui::GetMousePos() - mViewportMinBound;
		MousePos.y = viewportSize.y - MousePos.y;

		if (MousePos.x >= 0 && MousePos.y >= 0 && MousePos.x < viewportSize.x && MousePos.y < viewportSize.y) {
			auto readPixel = mFrameBuffer->ReadPixel(1, MousePos.x, MousePos.y);
			mHoveredEntity =  readPixel == -1 ? CEntity{} : CEntity{ TEntityID(readPixel), mActiveScene.get() };
		}
		mLifeSim2D->OnUpdate(_DeltaTime);
		mFrameBuffer->UnBind();
	}

	void CEditorLayer::OnEvent(CEvent& _Event)
	{
		mEditorCamera.OnEvent(_Event);
		CEventDispatcher dispatcher(_Event);
		dispatcher.Dispatch<CMouseButtonPressedEvent>(BIND_FN(&CEditorLayer::OnMousePressedEvent));
	}

	bool CEditorLayer::OnMousePressedEvent(const CMouseButtonPressedEvent& pE)
	{
		if (pE.GetMouseButton() == ARC_MOUSE_BUTTON_LEFT)
			if (mViewportHovered && !ImGuizmo::IsOver() && !SInput::IsKeyPressed(ARC_KEY_LEFT_ALT))
				mSceneHierachyPanel.SetSelectedEntity(mHoveredEntity);
		return false;
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
							mActiveScene = CreateRef<CScene>();
							mActiveScene->DeserializeFromText(filepath);

							mActiveScene->OnViewportResize({ (TUInt32)mViewportSize.x, (TUInt32)mViewportSize.y });
							mSceneHierachyPanel.SetContext(mActiveScene);
						}
					}
					if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					{
						auto filepath = CFileDialogs::SaveFile("ARC-Engine Scene (*.arc)\0*.arc\0");
						if (!filepath.empty())
							mActiveScene->SerializeToText(filepath);
					}
					if (ImGui::MenuItem("Exit")) CApplication::Get().Shutdown();
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			mSceneHierachyPanel.OnImGuiRender();

			ImGui::Begin("Settings");

			auto stats = CRenderer2D::GetStats();
			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
			std::string name = "None";
			if (mHoveredEntity)
				name = mHoveredEntity.GetComponent<CNameComponent>().Name;
			ImGui::Text("Hovered Entity: %s", name.c_str());

			ImGui::End();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			ImGui::Begin("Viewport");
			auto viewportMinRegion = (FVec2&)ImGui::GetWindowContentRegionMin();
			auto viewportMaxRegion = (FVec2&)ImGui::GetWindowContentRegionMax();
			auto viewportOffset = (FVec2&)ImGui::GetWindowPos();

			mViewportMinBound = viewportMinRegion + viewportOffset;
			mViewportMaxBound = viewportMaxRegion + viewportOffset;

			mViewportFocused = ImGui::IsWindowFocused();
			mViewportHovered = ImGui::IsWindowHovered();
			
			CApplication::Get().GetImGuiLayer()->SetBlockEvents(!mViewportFocused || !mViewportHovered);
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			TUInt32 textureID = mFrameBuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)textureID, ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			// Gizmos
			CEntity selectedEntity = mSceneHierachyPanel.GetSelectedEntity();
			if (selectedEntity)
			{
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();
				ImGuizmo::SetRect(mViewportMinBound.x, mViewportMinBound.y, mViewportMaxBound.x-mViewportMinBound.x, mViewportMaxBound.y - mViewportMinBound.y);
				
				if (selectedEntity.HasComponent<CTransform2DComponent>())
				{
					auto& entityTransform = selectedEntity.GetComponent<CTransform2DComponent>().Transform;
					auto entityTransformMatrix = SConvert<FGLMMat4, FTransform2D>::Conv(entityTransform);
					ImGuizmo::Manipulate(
						glm::value_ptr(mEditorCamera.GetViewMatrix()),
						glm::value_ptr(mEditorCamera.GetProjection()),
						ImGuizmo::OPERATION::TRANSLATE,
						ImGuizmo::LOCAL,
						glm::value_ptr(entityTransformMatrix)
					);
					entityTransform = SConvert<FTransform2D, FGLMMat4>::Conv(entityTransformMatrix);
				}
			}

			ImGui::End();
			ImGui::PopStyleVar();

			mLifeSim2D->OnGuiRender();
			ImGui::End();

		}
	}
}