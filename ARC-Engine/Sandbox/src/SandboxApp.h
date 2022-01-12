#pragma once
#include <string>
#include "ARC/Core/Application.h"
#include "ARC/Layers/Layer.h"
#include "ARC/Objects/Primitive2D.h"
#include "ARC/Renderer/Color.h"

namespace ARC { class CTexture2D; }
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
	
	float SQ_MoveSpeed = 3.f;
	float SQ_RotSpeed = 180.f;

	glm::vec4 SQ_Colour = ARC::CColor::Red;

	ARC::CPrimitive2D SQ_Data;

private:
	ARC::CShaderLibrary m_ShaderLibrary;

	ARC::TRef<ARC::CShader> m_TriangleShader;
	ARC::TRef<ARC::CVertexArray> m_TriangleVertexArray;
	
	ARC::TRef<ARC::CVertexArray> m_SquareVertexArray;
	ARC::TRef<ARC::CTexture2D> m_Texture;
	ARC::TRef<ARC::CTexture2D> m_TestTexture;

	ARC::COrthographicCameraBase m_Camera;
};

class CSandbox : public ARC::Core::CApplication
{
public:
	CSandbox();
	~CSandbox() {};

	inline virtual std::string GetAppName() override { return "Sandbox"; };
};
