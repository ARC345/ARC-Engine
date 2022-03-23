#include "PCH/arc_pch.h"
#include "Rock.h"
#include "ARC/Renderer/Texture.h"
#include "ARC/Renderer/Renderer2D.h"
#include "ARC/Helpers/Helpers.h"

namespace ARC {
	std::array< std::vector<std::shared_ptr<TVec2<uint8_t>>>, CRock::s_NumberOfRockSprites> CRock::SpriteCollisionData;

	void CRock::Load(FVec2 _Velocity, FVec2 _Position)
	{
		TString path = "assets/textures/Rock";
		SpriteIndex = (int)(HPR::Random::Float() * s_NumberOfRockSprites);
		path.append(std::to_string(SpriteIndex+1));
		path.append(TString(".png"));
		Sprite.Texture = CTexture2D::Create(path, true);

		if (SpriteCollisionData[SpriteIndex].empty())
		{
			for (uint8_t x = 0; x < 32; x++)
			{
				for (uint8_t y = 0; y < 32; y++)
				{
					if (Sprite.Texture->GetPixelColor({x, y}).a != 0 )
					{
						SpriteCollisionData[SpriteIndex].push_back(std::make_shared<TVec2<uint8_t>>(x, y));
					}
				}
			}
		}

		Sprite.Texture->ClearData();
		Sprite.Transform.Location = _Position;
		Sprite.Transform.Rotation = HPR::Random::Float(0, 360);
		ARC_CORE_TRACE("{0}", Sprite.GetRotation());
		Sprite.Transform.Scale = {
			HPR::Random::Float(0.3f, 0.7f),
			HPR::Random::Float(0.3f, 0.7f) };
		Velocity = _Velocity;

		auto TL = HPR::QUAD::GetScaledCorner<TopLeft>(Sprite.GetTransform());
		MinDist = FVec2::Dist(TL, Sprite.GetLocation());
	}

	void CRock::OnUpdate(float _DeltaTime)
	{
		Sprite.Transform.Location += Velocity * _DeltaTime;
		Sprite.Transform.Rotation += RotationVelocity * _DeltaTime;

		if (Sprite.GetLocation().x >2 || Sprite.GetLocation().y > 1.5 || Sprite.GetLocation().x < -2 || Sprite.GetLocation().y < -1.5) {
			CalculateRandomPosition();
			CalculateRandomVelocity();
		}
	}

	void CRock::CalculateRandomVelocity()
	{
		if (int(HPR::Random::Float() * 2)) {
			Velocity.x = 	HPR::Random::Float(0.2f, 1.f);
		} else {
			Velocity.x = 	HPR::Random::Float(-0.2f, -1.f);
		}
		
		if (int(HPR::Random::Float() * 2.f)) {
			Velocity.y = 	HPR::Random::Float(0.2f, 1.f);
		} else {
			Velocity.y = 	HPR::Random::Float(-0.2f, -1.f);
		}
		RotationVelocity = HPR::Random::Float(0.f, 3.f);
	}

	void CRock::CalculateRandomPosition()
	{
		if (int(HPR::Random::Float()*2)) {
			Sprite.Transform.Location.x = HPR::Random::Float(-1.8f, 1.8f);
			Sprite.Transform.Location.y = int(HPR::Random::Float() * 2.f) ? -1.25f : 1.25f;
		} else {
			Sprite.Transform.Location.x = int(HPR::Random::Float() * 2) ? -1.8f : 1.8f;
			Sprite.Transform.Location.y = HPR::Random::Float(-1.25f, 1.25f);
		}
	}

}