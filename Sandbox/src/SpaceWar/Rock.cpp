#include "PCH/arc_pch.h"
#include "Rock.h"
#include "ARC/Renderer/Texture.h"
#include "ARC/Renderer/Renderer2D.h"
#include "ARC/Helpers/Helpers.h"

namespace arc {
	std::array< std::vector<std::shared_ptr<vec2<uint8_t>>>, rock::number_of_rock_sprites> rock::SpriteCollisionData;

	void rock::Load(vec2 velocity, vec2 position)
	{
		string path = "assets/textures/Rock";
		SpriteIndex = (int)(HPR::Random::Float() * number_of_rock_sprites);
		path.Append(std::to_string(SpriteIndex+1));
		path.Append(string(".png"));
		Sprite.Texture = texture_2d::create(path, true);

		if (SpriteCollisionData[SpriteIndex].empty())
		{
			for (uint8_t x = 0; x < 32; x++)
			{
				for (uint8_t y = 0; y < 32; y++)
				{
					if (Sprite.Texture->get_pixel_color({x, y}).a != 0 )
					{
						SpriteCollisionData[SpriteIndex].push_back(std::make_shared<vec2<uint8_t>>(x, y));
					}
				}
			}
		}

		Sprite.Texture->ClearData();
		Sprite.Transform.Location = position;
		Sprite.Transform.Rotation = HPR::Random::Float(0, 360);
		ARC_CORE_TRACE("{0}", Sprite.get_rotation());
		Sprite.Transform.Scale = {
			HPR::Random::Float(0.3f, 0.7f),
			HPR::Random::Float(0.3f, 0.7f) };
		Velocity = velocity;

		auto TL = HPR::QUAD::get_corner<TopLeft>(Sprite.get_transform());
		MinDist = vec2::Dist(TL, Sprite.get_location());
	}

	void rock::on_update(float delta_time)
	{
		Sprite.Transform.Location += Velocity * delta_time;
		Sprite.Transform.Rotation += RotationVelocity * delta_time;

		if (Sprite.get_location().x >2 || Sprite.get_location().y > 1.5 || Sprite.get_location().x < -2 || Sprite.get_location().y < -1.5) {
			calculate_random_position();
			calculate_random_velocity();
		}
	}

	void rock::calculate_random_velocity()
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

	void rock::calculate_random_position()
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