#pragma once

#include "ARC/Renderer/Layer.h"
#include "ARC/Renderer/OrthographicCameraController.h"
#include "Panels/SceneHierarchyPanel.h"

namespace ARC { class CAtomExp; }

namespace ARC { class CFrameBuffer; }
namespace ARC { class CScene; }

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
	protected:
	private:
		uint8_t m_ViewportFocused : 1;
		uint8_t m_ViewportHovered : 1;
		FVec2 m_ViewportSize;
		TRef<CFrameBuffer> m_FrameBuffer;
		TRef<CScene> m_ActiveScene;
		TRef<CAtomExp> m_AtomExp;

		COrthographicCameraController m_CameraController;
		CSceneHierarchyPanel m_SceneHierachyPanel;
	};
}