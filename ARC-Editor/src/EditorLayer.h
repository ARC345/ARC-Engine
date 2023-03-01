#pragma once

#include "ARC/Renderer/Layer.h"
#include "ARC/Renderer/OrthographicCameraController.h"
#include "Panels/SceneHierarchyPanel.h"
#include "ARC/Scene/EditorCamera.h"

namespace ARC { class CLifeSim2D; }

namespace ARC { class CFrameBuffer; }
namespace ARC { class CScene; }
namespace ARC { class CMouseButtonPressedEvent; }
namespace ARC { class CMouseButtonReleasedEvent; }

namespace ARC {
	class CEditorLayer : public CLayer
	{
	public:
		CEditorLayer();
		virtual ~CEditorLayer() = default;
		virtual void OnGuiRender() override;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(float _DeltaTime) override;
		virtual void OnEvent(CEvent& _Event) override;

		bool OnMousePressedEvent(const CMouseButtonPressedEvent& pE);

	protected:
	private:
		TUInt8 mViewportFocused : 1;
		TUInt8 mViewportHovered : 1;
		FVec2 mViewportSize;
		FVec2 mViewportMinBound;
		FVec2 mViewportMaxBound;

		CEntity mHoveredEntity;
		TRef<CFrameBuffer> mFrameBuffer;
		TRef<CScene> mActiveScene;
		TRef<CLifeSim2D> mLifeSim2D;

		CEditorCamera mEditorCamera;
		COrthographicCameraController mCameraController;
		CSceneHierarchyPanel mSceneHierachyPanel;
	};
}