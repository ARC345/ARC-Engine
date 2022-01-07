#pragma once
#include <string>
#include "ARC/Core/Application.h"
#include "ARC/Layers/Layer.h"
#include "ARC/Objects/Primitive2D.h"

namespace ARC { class CKeyPressedEvent; }

namespace ARC { class CEvent; }
namespace ARC { class COrthographicCameraBase; }
namespace ARC { class CVertexArray; }
namespace ARC { class CIndexBuffer; }
namespace ARC { class CVertexBuffer; }
namespace ARC { class CShader; }


class CExampleLayer : public ARC::CLayer
{
public:
	CExampleLayer(); 
	void OnUpdate(float _DeltaTime) override;
	void OnEvent(ARC::CEvent& _event) override;

	bool OnKeyPressedEvent(ARC::CKeyPressedEvent& _Event);

	void OnGuiRender() override;

	inline ARC::COrthographicCameraBase& GetCam() { return m_Camera; }

public:
	float CamMoveSpeed = 10.f;
	float CamRotSpeed = 180.f;

	ARC::CPrimitive2D SQ_Data;

private:
	std::shared_ptr<ARC::CShader> m_TriangleShader;
	std::shared_ptr<ARC::CVertexArray> m_TriangleVertexArray;

	std::shared_ptr<ARC::CShader> m_SquareShader;
	std::shared_ptr<ARC::CVertexArray> m_SquareVertexArray;

	ARC::COrthographicCameraBase m_Camera;
};

class CSandbox : public ARC::Core::CApplication
{
public:
	CSandbox();
	~CSandbox() {};

	inline virtual std::string GetAppName() override { return "Sandbox"; };
};
