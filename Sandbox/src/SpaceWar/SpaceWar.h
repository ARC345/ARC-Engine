#pragma once
#include "ARC/Core/Application.h"
#include "ARC/Renderer/OrthographicCameraController.h"
#include "ARC/Objects/Primitive2D.h"
#include "ARC/Renderer/Color.h"
#include "ARC/Renderer/Layer.h"
#include <vector>
#include "imgui/imgui.h"

namespace ARC { class CMouseButtonPressedEvent; }
namespace ARC { class CVertexArray; }
namespace ARC { class CTexture2D; }
namespace ARC { class CPlayer; }
namespace ARC { class CShader; }
namespace ARC { class CEvent; }
namespace ARC { class CRock; }

enum class EGameState {
	Play = 0,
	MainMenu = 1,
	GameOver = 2
};

class CSpaceWar : public ARC::CLayer
{
public:
	CSpaceWar();
	virtual ~CSpaceWar() = default;

	virtual void OnAttach();
	virtual void OnDetach();

	virtual void OnUpdate(float _DeltaTime) override;
	virtual void OnEvent(ARC::CEvent& _Event) override;
	virtual void OnGuiRender() override;
	
	void ResetLevel();
	bool DetectCollision();
	
	bool OnMouseButtonPressed(ARC::CMouseButtonPressedEvent& _e);

	float prev = 0.f;
	EGameState GameState;
protected:
	ARC::TRef<ARC::CShader> m_Shader;
	ARC::TRef<ARC::CVertexArray> m_VertexArray;
	ARC::TRef<ARC::CPlayer> m_Player;
	//CParticleSystem2D m_ParticleSystem;
	std::vector<ARC::TRef<ARC::CRock>> m_Rocks;

	bool m_Blink = true; 
	ImFont* m_Font;

private:
	ARC::COrthographicCameraController m_CameraController;
};