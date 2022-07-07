#include "arc_pch.h"
#include "SpaceWar.h"
#include "ARC\Renderer\Buffer.h"
#include "ARC\Renderer\Shader.h"
#include "ARC\Renderer\VertexArray.h"
#include "imgui\imgui.h"
#include "glm\glm\gtc\type_ptr.inl"
#include "glm\glm\ext\matrix_transform.hpp"
#include "glm\glm\ext\matrix_float4x4.hpp"
#include "Platform\OpenGl\OpenGLShader.h"
#include "ARC\Renderer\Renderer.h"
#include "ARC\Renderer\RenderCommand.h"
#include "ARC/Renderer/Renderer2D.h"
#include "ARC/Renderer/Color.h"
#include "ARC/Renderer/Texture.h"
#include "ARC/Objects/Primitive2D.h"
#include "ARC/Types/vector.h"
#include "Player.h"
#include "Rock.h"
#include "ARC\Helpers\Math.h"
#include "ARC\Helpers\Helpers.h"
#include "ARC\Types\Transform2D.h"
#include "ARC\Core\Window.h"
#include "ARC\Events\MouseEvent.h"
#include "ARC\Events\Event.h"

static float i = 0.5;

CSpaceWar::CSpaceWar() :
	CLayer("SpaceWar"),
	m_CameraController(1280.f / 780.f, true)
{

}

void CSpaceWar::OnAttach()
{
	ARC::HPR::Random::Init();
	GameState = EGameState::MainMenu;
	m_Player = ARC::CreateRef<ARC::CPlayer>();
	m_Player->Load();
	m_Player->OwningLayer = this;
	ImGuiIO io = ImGui::GetIO();
	m_Font = io.Fonts->AddFontFromFileTTF("assets/fonts/Inconsolata-Medium.ttf", 120.0f);
}

void CSpaceWar::OnDetach()
{
	ARC::CInstrumentor::Get().EndSession();
}

bool CSpaceWar::DetectCollision()
{
	ARC_PROFILE_FUNCTION();
	ARC::FVec2 TLPP = ARC::HPR::QUAD::GetCorner<TopLeft>(m_Player->PlayerSprite.GetTransform());
	ARC::FVec2 TRPP = ARC::HPR::QUAD::GetCorner<TopRight>(m_Player->PlayerSprite.GetTransform());
	ARC::FVec2 BLPP = ARC::HPR::QUAD::GetCorner<BottomLeft>(m_Player->PlayerSprite.GetTransform());
	ARC::FVec2 BRPP = ARC::HPR::QUAD::GetCorner<BottomRight>(m_Player->PlayerSprite.GetTransform());

	std::vector<std::shared_ptr<ARC::FVec2>> cachedLocs;
	cachedLocs.reserve(m_Player->CollisionData.size());

	for (auto& PlayerCC : m_Player->CollisionData)
	{
		ARC::FVec2 PCCTop = ARC::Math::InterpF<Linear>(TLPP, TRPP, float(PlayerCC->x) / 32);
		ARC::FVec2 PCCBottom = ARC::Math::InterpF<Linear>(BLPP, BRPP, float(PlayerCC->x) / 32);
		cachedLocs.push_back(std::make_shared<ARC::FVec2>(
			ARC::Math::InterpF<Linear>(PCCBottom, PCCTop, float(PlayerCC->y) / 32)
		));
	}
	for (auto& rock : m_Rocks)
	{
		auto dist1 = ARC::FVec2::Dist(rock->Sprite.GetLocation(), m_Player->PlayerSprite.GetLocation());
		auto dist2 = ARC::Math::Max(rock->MinDist, m_Player->MinDist);
		if (dist1 < dist2)
		{
			ARC::FVec2 TLRP = ARC::HPR::QUAD::GetCorner<TopLeft>(rock->Sprite.GetTransform());
			ARC::FVec2 TRRP = ARC::HPR::QUAD::GetCorner<TopRight>(rock->Sprite.GetTransform());
			ARC::FVec2 BLRP = ARC::HPR::QUAD::GetCorner<BottomLeft>(rock->Sprite.GetTransform());
			ARC::FVec2 BRRP = ARC::HPR::QUAD::GetCorner<BottomRight>(rock->Sprite.GetTransform());

			for (auto& RockCC : ARC::CRock::SpriteCollisionData[rock->SpriteIndex])
			{
				ARC::FVec2 RCCTop = ARC::Math::InterpF<Linear>(TLRP, TRRP, float(RockCC->x) / 32);
				ARC::FVec2 RCCBottom = ARC::Math::InterpF<Linear>(BLRP, BRRP, float(RockCC->x) / 32);

				ARC::FVec2 RCCN = ARC::Math::InterpF<Linear>(RCCBottom, RCCTop, float(RockCC->y) / 32);

				for (auto& PlayerCC : cachedLocs)
				{
					if (ARC::FVec2::AlmostEqual(*PlayerCC, RCCN, 0.01f))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

void CSpaceWar::OnUpdate(float _DeltaTime)
{
	ARC_PROFILE_FUNCTION();
	
	static float TimePassed = 0;
	static float PrevSpawnTime = 0;
	TimePassed += _DeltaTime;
	if ((int)(TimePassed*10) % 10>8)
		m_Blink = !m_Blink;

	m_CameraController.OnUpdate(_DeltaTime);

	ARC::CRenderCommand::SetClearColour({ .1f, .1f, .1f, 1.f });
	ARC::CRenderCommand::Clear();
	
	ARC::CRenderer2D::BeginScene(m_CameraController.GetCamera());
	ARC::CRenderer2D::DrawQuad({0.f, 0.f}, 0.f, {10.f, 10.f}, 0.f, ARC::CColor(0.8f, 0.7f, 0.3f, 1.f));

	if (GameState == EGameState::Play)
	{
		if (TimePassed - PrevSpawnTime > 3) {
			if (!m_Rocks.empty() && m_Rocks.size() % 3 == 0)
			{
				m_Rocks.push_back(ARC::CreateRef<ARC::CRock>());
				i += 0.0001f;
				m_Rocks.back()->Sprite.Transform.ZOrder = i;
				m_Rocks.back()->CalculateRandomPosition();
			
				m_Rocks.back()->Load({ 0,0 }, { 0,0 });
				m_Rocks.back()->CalculateRandomPosition();
				m_Rocks.back()->Velocity = (m_Player->PlayerSprite.GetLocation() / m_Player->PlayerSprite.GetLocation().Length())*-1.f;
			}
			else
			{
				m_Rocks.push_back(ARC::CreateRef<ARC::CRock>());
				i += 0.01f;
				m_Rocks.back()->Sprite.Transform.ZOrder = i;
				m_Rocks.back()->Load({ 0,0 }, { 0,0 });
				m_Rocks.back()->CalculateRandomPosition();
				m_Rocks.back()->CalculateRandomVelocity();
			}
			PrevSpawnTime = TimePassed;
		}
		if (DetectCollision())
		{
			GameState = EGameState::GameOver;
			m_Blink=true;
		}
		m_Player->OnUpdate(_DeltaTime);
		for (auto& rock : m_Rocks) {
			rock->OnUpdate(_DeltaTime);
		}
	}
	ARC::CRenderer2D::DrawQuad(m_Player->PlayerSprite);
	m_Player->ParticleSystem.OnRender();
	for (auto& rock : m_Rocks) {
		ARC::CRenderer2D::DrawQuad(rock->Sprite);
	}
	ARC::CRenderer2D::EndScene();
}

void CSpaceWar::OnEvent(ARC::CEvent& _Event)
{
	ARC_PROFILE_FUNCTION();
	m_Player->OnEvent(_Event);
	m_CameraController.OnEvent(_Event);

	ARC::CEventDispatcher dispatcher(_Event);
	dispatcher.Dispatch<ARC::CMouseButtonPressedEvent>(BIND_FN(&CSpaceWar::OnMouseButtonPressed));
}

bool CSpaceWar::OnMouseButtonPressed(ARC::CMouseButtonPressedEvent& _e)
{
	if (GameState == EGameState::GameOver)
		ResetLevel();

	GameState = EGameState::Play;
	return false;
}

void CSpaceWar::OnGuiRender()
{
	ARC_PROFILE_FUNCTION();


	switch (GameState)
	{
		case EGameState::Play:
		{
			uint32_t playerScore = (uint32_t)m_Rocks.size();
			std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
			ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, ImGui::GetWindowPos(), 0xffffffff, scoreStr.c_str());
			break;
		}
		case EGameState::MainMenu:
		{
			auto pos = ImGui::GetWindowPos();
			auto width = ARC::CApplication::Get().GetWindow().GetWidth();
			auto height = ARC::CApplication::Get().GetWindow().GetHeight();
			pos.x += width * 0.5f - 460.0f;
			pos.y += 50.0f;
			if (m_Blink)
				ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");
			break;
		}
		case EGameState::GameOver:
		{
			auto pos = ImGui::GetWindowPos();
			auto width = ARC::CApplication::Get().GetWindow().GetWidth();
			auto height = ARC::CApplication::Get().GetWindow().GetHeight();
			pos.x += width * 0.5f - 460.0f;
			pos.y += 50.0f;
			if (m_Blink)
				ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");

			pos.x += 305.0f;
			pos.y += 150.0f;
			uint32_t playerScore = (uint32_t)m_Rocks.size();
			std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
			ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, pos, 0xffffffff, scoreStr.c_str());
			break;
		}
	}
}

void CSpaceWar::ResetLevel()
{
	GameState = EGameState::Play;
	i=0.5;
	m_Player->Reset();
	m_Rocks.clear();
}
