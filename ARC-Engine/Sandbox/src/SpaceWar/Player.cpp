#include "PCH/arc_pch.h"
#include "Player.h"
#include "ARC/Renderer/Texture.h"
#include "ARC/Renderer/Renderer2D.h"
#include "ARC/Types/Transform2D.h"
#include "ARC/Objects/Primitive2D.h"
#include "ARC/Events/Event.h"
#include "ARC/Events/MouseEvent.h"
#include <cmath>
#include "glm\glm\ext\matrix_transform.hpp"
#include "glm\glm\ext\matrix_float4x4.hpp"
#include "ARC/Core/Application.h"
#include "ARC/Core/Window.h"
#include "ARC/Input/KeyCodes.h"
#include "ARC/Events/KeyEvent.h"
#include "ARC/Input/Input.h"
#include "glm/ext/scalar_common.hpp"
#include <algorithm>
#include "ARC/Helpers/Math.h"
#include "SpaceWar.h"
#include "ARC/Objects/ParticleModifier2D.h"
#include "ARC/Objects/ParticleSystem2D.h"

namespace ARC {
	CPlayer::CPlayer() :
		Speed(1)
	{
		ParticleSystem.Defaults.bIsActive = false;
		ParticleSystem.Defaults.Scale = { 0.05f, 0.05f};
		ParticleSystem.Defaults.Color = ARC::CColor::Red;

		ParticleSystem.Defaults.Velocity = { 0.f, 2.f };
		ParticleSystem.Defaults.Life = 0.5f;
		ParticleSystem.Defaults.ZOrder = 0.1f;
		ParticleSystem.AddParticleModifier(ARC::EVariationApplicationTime::OnEmit, new ARC::CParticleVariationModifier2D(&ARC::CParticle2D::Life, 0.05f));
		//ParticleSystem->AddParticleModifier(ARC::EVariationApplicationTime::OnEmit, new ARC::CParticleVariationModifier2D(&ARC::CParticle2D::Color, ARC::CColor(0.1f, 0.1f, 0.1f, 0.f)));
		//ParticleSystem.AddParticleModifier(ARC::EVariationApplicationTime::OnEmit, new ARC::CParticleVariationModifier2D(&ARC::CParticle2D::Location, ARC::FVec2(0.05, 0.05)));
		ParticleSystem.AddParticleModifier(ARC::EVariationApplicationTime::OnEmit, new ARC::CParticleVariationModifier2D(&ARC::CParticle2D::Scale, ARC::FVec2(0.02f, 0.02f)));
		//ParticleSystem.AddParticleModifier(ARC::EVariationApplicationTime::OnEmit, new ARC::CParticleVariationModifier2D(&ARC::CParticle2D::Velocity, ARC::FVec2(0.6f, 1.0f)));
		ParticleSystem.AddParticleModifier(ARC::EVariationApplicationTime::OnUpdate, new ARC::CParticleLifetimeModifier2D(&ARC::CParticle2D::Color, ARC::CColor(1.f, 1.f, 0.f, 1.f)));
		ParticleSystem.AddParticleModifier(ARC::EVariationApplicationTime::OnUpdate, new ARC::CParticleLifetimeModifier2D<Flip<Quadratic>, ARC::FVec2>(&ARC::CParticle2D::Scale, ARC::FVec2::ZeroVector));

	}
	
	void CPlayer::Load()
	{
		PlayerSprite.Texture = CTexture2D::Create("assets/textures/arcShip.png", true);
		PlayerSprite.Transform.Location = { 0.1f, 0.1f};
		PlayerSprite.Transform.Scale = {0.1f, 0.1f};
		PlayerSprite.Transform.ZOrder = 0.2f;

		auto TL = HPR::QUAD::GetScaledCorner<TopLeft>(PlayerSprite.GetTransform());
		MinDist = FVec2::Dist(TL, PlayerSprite.GetLocation());

		for (uint8_t x = 0; x < 32; x++)
		{
			for (uint8_t y = 0; y < 32; y++)
			{
				if (PlayerSprite.Texture->GetPixelColor({ x, y }).a != 0)
				{
					CollisionData.push_back( std::make_shared<TVec2<uint8_t>>( x, y ));
				}
			}
		}

		PlayerSprite.Texture->ClearData();
	}

	void CPlayer::Reset()
	{
		PlayerSprite.Transform.Location = { 0.f, 0.f };
		PlayerSprite.Transform.Scale = { 0.1f, 0.1f };
		PlayerSprite.Transform.ZOrder = 0.2f;
		Velocity = FVec2::ZeroVector;
	}

	void CPlayer::OnUpdate(float _DeltaTime)
	{
		if (auto* SWL = (CSpaceWar*)OwningLayer)
		{
			if (SWL && SWL->GameState == EGameState::Play)
			{
				float lerpalpha = 0;
				Velocity.x *= 0.97f*_DeltaTime*60.f;
				Velocity.y *= 0.97f*_DeltaTime*60.f;

				if (CInput::IsKeyPressed(ARC_KEY_A) || CInput::IsKeyPressed(ARC_KEY_W) || CInput::IsKeyPressed(ARC_KEY_S) || CInput::IsKeyPressed(ARC_KEY_D) || CInput::IsKeyPressed(ARC_KEY_RIGHT) || CInput::IsKeyPressed(ARC_KEY_LEFT) || CInput::IsKeyPressed(ARC_KEY_UP) || CInput::IsKeyPressed(ARC_KEY_DOWN)) {
					lerpalpha += _DeltaTime;
					if (CInput::IsKeyPressed(ARC_KEY_A) || CInput::IsKeyPressed(ARC_KEY_LEFT)) {
						if (CInput::IsKeyPressed(ARC_KEY_D) || CInput::IsKeyPressed(ARC_KEY_RIGHT)) Velocity.x = 0;
						else Velocity.x = -1;
					}
					else if (CInput::IsKeyPressed(ARC_KEY_D) || CInput::IsKeyPressed(ARC_KEY_RIGHT)) Velocity.x = 1;
					if (CInput::IsKeyPressed(ARC_KEY_W) || CInput::IsKeyPressed(ARC_KEY_UP)) {
						if (CInput::IsKeyPressed(ARC_KEY_S) || CInput::IsKeyPressed(ARC_KEY_DOWN)) Velocity.y = 0;
						else Velocity.y = 1;
					}
					else if (CInput::IsKeyPressed(ARC_KEY_S) || CInput::IsKeyPressed(ARC_KEY_DOWN)) Velocity.y = -1;

					FVec2 emissionPoint = { 0.0f, -0.05f };
					float s = std::sin(PlayerSprite.GetRotation());
					float c = std::cos(PlayerSprite.GetRotation());

					FVec2 rotated = FVec2(-emissionPoint.y*s, emissionPoint.y * c);
					ParticleSystem.Defaults.Location = PlayerSprite.GetLocation();
					ParticleSystem.Defaults.Velocity = Math::InterpF<Linear>(ParticleSystem.Defaults.Velocity, (Velocity + rotated)* -1, lerpalpha);
					ParticleSystem.Emit();
				}
				else {
					lerpalpha-=_DeltaTime;
					lerpalpha = std::clamp(lerpalpha, 0.f, 1.f);
				}

				PlayerSprite.Transform.Location.x += Velocity.x * Speed * _DeltaTime;
				PlayerSprite.Transform.Location.y += Velocity.y * Speed * _DeltaTime;

				PlayerSprite.Transform.Location.x = std::clamp<float>(PlayerSprite.Transform.Location.x, -1.6f, 1.6f);
				PlayerSprite.Transform.Location.y = std::clamp<float>(PlayerSprite.Transform.Location.y, -0.94f, 0.94f);
				ParticleSystem.OnUpdate(_DeltaTime);
			}
 			const FVec2 mouse_location = CInput::GetMouseXY();
			auto window_height = Core::CApplication::Get().GetWindow().GetHeight();
			auto window_width = Core::CApplication::Get().GetWindow().GetWidth();

 			const float y = -((mouse_location.y / window_height)*2-1);
 			const float x = 1.6666f *((mouse_location.x / window_width)*2-1);
 			
			float x_angle = Math::Conv<Radians, Degree>(atan2(
 					y - PlayerSprite.Transform.Location.y,
 					x - PlayerSprite.Transform.Location.x)
 				);
			PlayerSprite.Transform.Rotation = Math::Conv<Degree, Radians>(x_angle - 90.f);
		}
	}

	void CPlayer::OnEvent(CEvent& _Event)
	{
		CEventDispatcher dispatcher(_Event);
	}
}
