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
#include "ARC/Events/KeyEvent.h"

namespace ARC {
	static CEntity ESquare;
	static CEntity ECamera;

	CEditorLayer::CEditorLayer() :
		CLayer("EditorLayer"),
		mCameraController(1280.f / 780.f, true)
		#ifdef ARC_DRAW_DEBUG_SHAPES
		, DrawDebugShapes(1u)
		#endif // ARC_DRAW_DEBUG_SHAPES
		
	{
	}

	void CEditorLayer::OnAttach()
	{
		mPlayButtonTexture = CTexture2D::Create(std::filesystem::path("resources/icons/PlayButton.png"));
		mStopButtonTexture = CTexture2D::Create(std::filesystem::path("resources/icons/StopButton.png"));
		mPauseButtonTexture = CTexture2D::Create(std::filesystem::path("resources/icons/PauseButton.png"));
		mSimulateButtonTexture = CTexture2D::Create(std::filesystem::path("resources/icons/SimulateButton.png"));

		SFrameBufferSpecification frame_buffer_specs;
		frame_buffer_specs.Width = 1280;
		frame_buffer_specs.Height = 720;
		frame_buffer_specs.Attachments = { EFrameBufferTextureFormat::RGBA8, EFrameBufferTextureFormat::RED_INTEGER, EFrameBufferTextureFormat::DEPTH24STENCIL8 };
		mFrameBuffer = CFrameBuffer::Create(frame_buffer_specs);

		mActiveScene = CreateRef<CScene>();

		mEditorScene = mActiveScene;

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

		if (mIsViewportHovered && mIsViewportFocused)	mCameraController.OnUpdate(_DeltaTime);
		
		SRenderer2D::ResetStats();

		mFrameBuffer->Bind();

		CRenderCommand::SetClearColour({ .1f, .1f, .1f, 1.f });
		CRenderCommand::Clear();

		mFrameBuffer->ClearColorAttachment(1, -1);

		switch (mSceneState) {
		case ESceneState::Edit:
			mActiveScene->OnUpdateEditor(_DeltaTime, mEditorCamera);
			break;
		case ESceneState::Play:
			mActiveScene->OnUpdateRuntime(_DeltaTime);
			break;
		case ESceneState::Simulate:
			mActiveScene->OnUpdateSimulation(_DeltaTime, mEditorCamera);
			break;
		}

		auto viewportSize = mViewportMaxBound-mViewportMinBound;
		auto MousePos = (FVec2&)ImGui::GetMousePos() - mViewportMinBound;
		MousePos.y = viewportSize.y - MousePos.y;

		if (MousePos.x >= 0 && MousePos.y >= 0 && MousePos.x < viewportSize.x && MousePos.y < viewportSize.y) {
			auto readPixel = mFrameBuffer->ReadPixel(1, MousePos.x, MousePos.y);
			mHoveredEntity =  readPixel == -1 ? CEntity{} : CEntity{ TEntityID(readPixel), mActiveScene.get() };
		}
		mLifeSim2D->OnUpdate(_DeltaTime);
		OnOverlayRender();
		mFrameBuffer->UnBind();

		mEditorCamera.OnUpdate(_DeltaTime);


	}

	void CEditorLayer::OnEvent(CEvent& _Event)
	{
		mEditorCamera.OnEvent(_Event);
		CEventDispatcher dispatcher(_Event);
		dispatcher.Dispatch<CKeyPressedEvent>(BIND_FN(&CEditorLayer::OnKeyPressed));
		dispatcher.Dispatch<CMouseButtonPressedEvent>(BIND_FN(&CEditorLayer::OnMousePressedEvent));
	}
	bool CEditorLayer::OnKeyPressed(CKeyPressedEvent& pE)
	{
		if (pE.GetRepeatCount() > 0)
			return false;

		bool control = SInput::IsKeyPressed(EKey::LeftControl) || SInput::IsKeyPressed(EKey::RightControl);
		bool shift = SInput::IsKeyPressed(EKey::LeftShift) || SInput::IsKeyPressed(EKey::RightShift);

		switch (pE.GetKeyCode())
		{
		// Scene
		case EKey::N:
		{
			if (control)
				NewScene();

			break;
		}
		case EKey::O:
		{
			if (control)
				OpenScene();

			break;
		}
		case EKey::S:
		{
			if (control) {
				if (shift)
					SaveSceneAs();
				else
					SaveScene();
			}
			break;
		}
		// Editor
		case EKey::D:
		{
			switch (mSceneState) {
			case ESceneState::Edit:
				if (control && mSceneHierachyPanel.GetSelectedEntity()) 
					mSceneHierachyPanel.GetSelectedEntity().Duplicate(); // @TODO: Rename
				
				break;
			};
			break;
		}
		
		// Gizmos
		case EKey::Q:
		{
			if (!ImGuizmo::IsUsing())
				mGuizmoType = -1;
			break;
		}
		case EKey::W:
		{
			if (!ImGuizmo::IsUsing())
				mGuizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		}
		case EKey::E:
		{
			if (!ImGuizmo::IsUsing())
				mGuizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		}
		case EKey::R:
		{
			if (!ImGuizmo::IsUsing())
				mGuizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}
		}
	}
	bool CEditorLayer::OnMousePressedEvent(CMouseButtonPressedEvent& pE)
	{
		if (pE.GetMouseButton() == EMouse::ButtonLeft)
			if (mIsViewportHovered && !ImGuizmo::IsOver() && !SInput::IsKeyPressed(EKey::LeftAlt))
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
						auto filepath = SFileDialogs::OpenFile("ARC-Engine Scene (*.arc)\0*.arc\0");
						if (!filepath.empty())
							OpenScene(filepath);
					}
					if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					{
						auto filepath = SFileDialogs::SaveFile("ARC-Engine Scene (*.arc)\0*.arc\0");
						if (!filepath.empty())
							mActiveScene->SerializeToText(filepath);
					}
					if (ImGui::MenuItem("Exit")) CApplication::Get().Shutdown();
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			mSceneHierachyPanel.OnImGuiRender();
			mContentBrowserPanel.OnImGuiRender();

			ImGui::Begin("Settings");

			auto stats = SRenderer2D::GetStats();
			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

			TString name = "None";

			if (mHoveredEntity && mHoveredEntity.HasComponent<CNameComponent>())
				name = mHoveredEntity.GetComponent<CNameComponent>().Name;

			ImGui::Text("Hovered Entity: %s", name.c_str());

			if (ImGui::TreeNode("Debug"))
			{
				if (ImGui::TreeNode("Colliders"))
				{
					ImGui::Checkbox("Draw Debug Colliders", &DrawDebugShapes);
					ImGui::Separator();


					ImGui::ColorEdit4("Circle Line Color", DebugCircleColliderColor.Data());
					ImGui::DragFloat("Circle Line Thickness", &DebugCircleColliderThickness);
					ImGui::Separator();

					ImGui::ColorEdit4("Box Line Color", DebugBoxColliderColor.Data());
					ImGui::DragFloat("Box Line Thickness", &DebugBoxColliderThickness);

					ImGui::TreePop();
				}
				ImGui::TreePop();
			}

			ImGui::End();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 5, 5 });
			ImGui::Begin("Viewport");
			auto viewportMinRegion = (FVec2&)ImGui::GetWindowContentRegionMin();
			auto viewportMaxRegion = (FVec2&)ImGui::GetWindowContentRegionMax();
			auto viewportOffset = (FVec2&)ImGui::GetWindowPos();

			mViewportMinBound = viewportMinRegion + viewportOffset;
			mViewportMaxBound = viewportMaxRegion + viewportOffset;

			mIsViewportFocused = ImGui::IsWindowFocused();
			mIsViewportHovered = ImGui::IsWindowHovered();
			
			CApplication::Get().GetImGuiLayer()->SetBlockEvents(!mIsViewportFocused || !mIsViewportHovered);
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			TUInt64 textureID = mFrameBuffer->GetColorAttachmentRendererID();
			
			ImGui::Image((void*)textureID, ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			if (ImGui::BeginDragDropTarget())
			{
				if (const auto* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const auto* path = (const wchar_t*)payload->Data;
					OpenScene(std::filesystem::path("assets")/std::filesystem::path(path));
				}
				ImGui::EndDragDropTarget();
			}

			// Gizmos
			CEntity selectedEntity = mSceneHierachyPanel.GetSelectedEntity();
			if (selectedEntity && mGuizmoType != -1)
			{
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();
				ImGuizmo::SetRect(mViewportMinBound.x, mViewportMinBound.y, mViewportMaxBound.x-mViewportMinBound.x, mViewportMaxBound.y - mViewportMinBound.y);
				
				if (selectedEntity.HasComponent<CTransform2DComponent>())
				{
					// Snapping
					bool snap = SInput::IsKeyPressed(EKey::LeftControl);
					float snapValue = 0.5f; // Snap to 0.5m for translation/scale

					// Snap to 45 degrees for rotation
					if (mGuizmoType == ImGuizmo::OPERATION::ROTATE)
						snapValue = 45.0f;

					float snapValues[3] = { snapValue, snapValue, snapValue };

					auto& entityTransform = selectedEntity.GetComponent<CTransform2DComponent>().Transform;
					auto entityTransformMatrix = SConvert<FGLMMat4, FTransform2D>::Conv(entityTransform);
					ImGuizmo::Manipulate(
						glm::value_ptr(mEditorCamera.GetViewMatrix()),
						glm::value_ptr(mEditorCamera.GetProjection()),
						(ImGuizmo::OPERATION)mGuizmoType,
						ImGuizmo::LOCAL,
						glm::value_ptr(entityTransformMatrix),
						nullptr,
						snap ? snapValues : nullptr
					);
					if (ImGuizmo::IsUsing())
						entityTransform = SConvert<FTransform2D, FGLMMat4>::Conv(entityTransformMatrix);
				}
			}

			ImGui::End();
			ImGui::PopStyleVar();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 2));
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0.1));
			const auto& tmp = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { tmp.x,tmp.y,tmp.z,tmp.w / 2 });
			const auto& tmp2 = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { tmp2.x,tmp2.y,tmp2.z,tmp2.w / 2 });
			ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse);
			auto buttonSize = ImGui::GetWindowHeight()-10;
			switch (mSceneState) {
			case ESceneState::Edit:
				ImGui::SetCursorPosX((ImGui::GetContentRegionMax().x) * 0.5f - buttonSize);

				if (ImGui::ImageButton((ImTextureID)(uint64_t)mPlayButtonTexture->GetRendererID(), ImVec2(buttonSize, buttonSize), ImVec2(0, 0), ImVec2(1, 1), 0)) {
					SetSceneState(ESceneState::Play);
				}
				ImGui::SameLine();
				if (ImGui::ImageButton((ImTextureID)(uint64_t)mSimulateButtonTexture->GetRendererID(), ImVec2(buttonSize, buttonSize), ImVec2(0, 0), ImVec2(1, 1), 0)) {
					SetSceneState(ESceneState::Simulate);
				}
				break;
			case ESceneState::Play: case ESceneState::Simulate:
				ImGui::SetCursorPosX((ImGui::GetContentRegionMax().x - buttonSize) * 0.5f);
				if (ImGui::ImageButton((ImTextureID)(uint64_t)mStopButtonTexture->GetRendererID(), ImVec2(buttonSize, buttonSize), ImVec2(0, 0), ImVec2(1, 1), 0)) {
					SetSceneState(ESceneState::Edit);
				}
				break;
			}

			ImGui::End();
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(3);
			mLifeSim2D->OnGuiRender();
			ImGui::End();

		}
	}

	void CEditorLayer::NewScene()
	{
		mActiveScene = CreateRef<CScene>();
		mActiveScene->OnViewportResize(TVec2<uint32_t>(mViewportSize.x, mViewportSize.y));
		mSceneHierachyPanel.SetContext(mActiveScene);
		mEditorScenePath = std::filesystem::path();
	}

	void CEditorLayer::OpenScene()
	{
		OpenScene(SFileDialogs::OpenFile("ARC-Engine Scene (*.arc)\0*.arc\0"));
	}

	void CEditorLayer::OpenScene(const std::filesystem::path& pFilepath)
	{
		if (pFilepath.empty()) return;
		if (pFilepath.extension().string() != ".arc") return;

		mEditorScene = CreateRef<CScene>();
		mEditorScene->OnViewportResize(TVec2<uint32_t>(mViewportSize.x, mViewportSize.y));
		mEditorScene->DeserializeFromText(pFilepath);

		SetSceneState(ESceneState::Edit);

		mEditorScenePath = pFilepath;
	}

	void CEditorLayer::SaveSceneAs()
	{
		auto filepath = SFileDialogs::SaveFile("ARC-Engine Scene (*.arc)\0*.arc\0");
		if (filepath.empty()) return;
		mEditorScenePath = filepath;
		mActiveScene->SerializeToText(filepath);
	}

	void CEditorLayer::SaveScene()
	{
		if (mEditorScenePath.empty()) SaveSceneAs();
		else mActiveScene->SerializeToText(mEditorScenePath);
	}

	void CEditorLayer::SetSceneState(ESceneState pNewState)
	{
		if (mSceneState == ESceneState::Play || mSceneState == ESceneState::Simulate) {
			mActiveScene->OnSetSceneState((TUInt8)mSceneState, 0);
		}

		mActiveScene = CScene::Copy(mEditorScene);
		mActiveScene->OnSetSceneState(0, (TUInt8)pNewState);
		mSceneState = pNewState;

		mSceneHierachyPanel.SetContext(mActiveScene);
	}

	void CEditorLayer::OnOverlayRender()
	{
		#ifdef ARC_DRAW_DEBUG_SHAPES
		SRenderer2D::EndScene();
		switch (mSceneState) {
		case ESceneState::Edit: case ESceneState::Simulate:
			SRenderer2D::BeginScene(mEditorCamera);
			break;
		case ESceneState::Play:
			if (auto e = mActiveScene->GetPrimaryCameraEntity())
				SRenderer2D::BeginScene(e.GetComponent<CCameraComponent>().Camera, e.GetComponent<CTransform2DComponent>().Transform);
			break;
		}
		mActiveScene->FilterByComponents<CTransform2DComponent, CCircleCollider2DComponent>().each([&](auto e, auto& transformComponent, auto& circleColliderComponent) {
			if (DrawDebugShapes)
			{
				SRenderer2D::DrawCircle(transformComponent.Transform.Location+FVec3(circleColliderComponent.Offset.x, circleColliderComponent.Offset.y, 0.001), transformComponent.Transform.Rotation, FVec2(circleColliderComponent.Radius) * transformComponent.Transform.Scale.x * 2.f, DebugCircleColliderColor, DebugCircleColliderThickness, 0.995, (int)e);
			}
			});
		mActiveScene->FilterByComponents<CTransform2DComponent, CBoxCollider2DComponent>().each([&](auto e, auto& transformComponent, auto& boxColliderComponent) {
			if (DrawDebugShapes)
			{
				float lT = SRenderer2D::GetLineThickness();
				SRenderer2D::SetLineThickness(DebugBoxColliderThickness);
				SRenderer2D::DrawRect(
					transformComponent.Transform.Location + FVec3(boxColliderComponent.Offset.x, boxColliderComponent.Offset.y, 0.001),
					transformComponent.Transform.Rotation,
					boxColliderComponent.Size * transformComponent.Transform.Scale * 2.f,
					DebugBoxColliderColor,
					(int)e);
				SRenderer2D::SetLineThickness(lT);
			}
			});
		SRenderer2D::EndScene();
		#endif // ARC_DRAW_DEBUG_SHAPES
		
	}
}