#pragma once
#include "ARC/Core/Application.h"
#include "ARC/Renderer/Layer.h"
#include "ARC/Renderer/OrthographicCameraController.h"
#include "ARC/Renderer/ParticleSystem2D.h"
#include "ARC/Objects/Primitive2D.h"

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
		COrthographicCameraController m_CameraController;
	};
}