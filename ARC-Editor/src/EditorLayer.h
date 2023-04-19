#pragma once

#include "ARC/Renderer/Layer.h"
#include "ARC/Renderer/OrthographicCameraController.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "ARC/Scene/EditorCamera.h"
#include "ARC/Core/Macros.h"

namespace ARC { class CLifeSim2D; }
namespace ARC { class CFrameBuffer; }
namespace ARC { class CScene; }
namespace ARC { class CKeyPressedEvent; }
namespace ARC { class CMouseButtonPressedEvent; }
namespace ARC { class CMouseButtonReleasedEvent; }

namespace ARC {
	enum class ESceneState : TUInt8 {
		None = 0,
		Edit=1,
		Play=2,
		Simulate=3
	};

	class CEditorLayer : public CLayer
	{
	public:
		CEditorLayer();
		virtual ~CEditorLayer() = default;
		virtual void OnGuiRender() override;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(float pDeltaTime) override;
		virtual void OnEvent(CEvent& pEvent) override;

		bool OnKeyPressed(CKeyPressedEvent& pE);
		bool OnMousePressedEvent(CMouseButtonPressedEvent& pE);

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& pFilepath);
		void SaveSceneAs();
		void SaveScene();

		void SetSceneState(ESceneState pNewState);

		void OnOverlayRender();

	public:

	#ifdef ARC_DRAW_DEBUG_SHAPES
		bool DrawDebugShapes;
		float DebugCircleColliderThickness = 0.1f;
		FColor4 DebugCircleColliderColor = FColor4::Green();
		float DebugBoxColliderThickness = 0.1f;
		FColor4 DebugBoxColliderColor = FColor4::Green();
	#endif
		
	protected:
	private:
		TUInt8 mIsViewportFocused : 1;
		TUInt8 mIsViewportHovered : 1;
		FVec2 mViewportSize;
		FVec2 mViewportMinBound;
		FVec2 mViewportMaxBound;

		int mGuizmoType = -1;
		CEntity mHoveredEntity;
		TRef<CFrameBuffer> mFrameBuffer;
		TRef<CScene> mActiveScene;
		TRef<CScene> mEditorScene;
		TRef<CLifeSim2D> mLifeSim2D;
		
		std::filesystem::path mEditorScenePath;

		CEditorCamera mEditorCamera;
		COrthographicCameraController mCameraController;
		CSceneHierarchyPanel mSceneHierachyPanel;
		CContentBrowserPanel mContentBrowserPanel;

		ESceneState mSceneState = ESceneState::Edit;
		TRef<CTexture2D> mPlayButtonTexture;
		TRef<CTexture2D> mStopButtonTexture;
		TRef<CTexture2D> mSimulateButtonTexture;
		TRef<CTexture2D> mPauseButtonTexture;
	};
}