#pragma once
#include "ARC/Core/Application.h"
#include "ARC/Renderer/Layer.h"
#include "ARC/Objects/OrthographicCameraController.h"
#include "ARC/Objects/Primitive2D.h"

namespace ARC { class CTexture2D; }

namespace ARC { class CVertexArray; }

namespace ARC { class CShader; }

namespace ARC { class CEvent; }

class CSandbox2D : public ARC::CLayer
{
public:
	CSandbox2D();
	virtual ~CSandbox2D() = default;

	virtual void OnAttach();
	virtual void OnDetach();

	virtual void OnUpdate(float _DeltaTime) override;
	virtual void OnEvent(ARC::CEvent& _Event) override;
	virtual void OnGuiRender() override;

	float prev = 0.f;
protected:
	ARC::TRef<ARC::CShader> m_Shader;
	ARC::TRef<ARC::CVertexArray> m_VertexArray;
	ARC::TRef<ARC::CTexture2D> m_CheckerboardTexture;
	
	ARC::CColor SQ_Colour = ARC::CColor::Blue;

private:
	ARC::COrthographicCameraController m_CameraController;
};