#include "arc_pch.h"

#include "ARC/Types/Transform2D.h"
#include "ARC/Objects/Primitive2D.h"

#include "glm/glm/ext/matrix_transform.hpp"
#include "glm/glm/ext/matrix_float4x4.hpp"
#include "glm/ext/scalar_common.hpp"

#include "ARC/Core/Window.h"
#include "ARC/Core/Application.h"

#include "ARC/Input/Input.h"
#include "ARC/Input/KeyCodes.h"

#include "Player.h"
#include "SpaceWar.h"

#include "ARC/Events/Event.h"
#include "ARC/Events/KeyEvent.h"
#include "ARC/Events/MouseEvent.h"

#include "ARC/Renderer/Texture.h"
#include "ARC/Renderer/Renderer2D.h"
#include "ARC/Renderer/ParticleSystem2D.h"
#include "ARC/Renderer/ParticleModifier2D.h"

namespace arc {
	player::player() :
		Speed(1)
	{
		ParticleSystem.Defaults.bIsActive = false;
		ParticleSystem.Defaults.Scale = { 0.05f, 0.05f};
		ParticleSystem.Defaults.Color = arc::CColor::Red;

		ParticleSystem.Defaults.Velocity = { 0.f, 2.f };
		ParticleSystem.Defaults.Life = 0.5f;
		ParticleSystem.Defaults.ZOrder = 0.1f;
		ParticleSystem.AddParticleModifier(arc::variation_application_time::OnEmit, new arc::particle_variation_modifier_2d(&arc::particle_2d::Life, 0.05f));
		//ParticleSystem->AddParticleModifier(arc::variation_application_time::OnEmit, new arc::particle_variation_modifier_2d(&arc::particle_2d::Color, arc::CColor(0.1f, 0.1f, 0.1f, 0.f)));
		//ParticleSystem.AddParticleModifier(arc::variation_application_time::OnEmit, new arc::particle_variation_modifier_2d(&arc::particle_2d::Location, arc::vec2(0.05, 0.05)));
		ParticleSystem.AddParticleModifier(arc::variation_application_time::OnEmit, new arc::particle_variation_modifier_2d(&arc::particle_2d::Scale, arc::vec2(0.02f, 0.02f)));
		//ParticleSystem.AddParticleModifier(arc::variation_application_time::OnEmit, new arc::particle_variation_modifier_2d(&arc::particle_2d::Velocity, arc::vec2(0.6f, 1.0f)));
		ParticleSystem.AddParticleModifier(arc::variation_application_time::on_update, new arc::particle_lifetime_modifier_2d(&arc::particle_2d::Color, arc::CColor(1.f, 1.f, 0.f, 1.f)));
		ParticleSystem.AddParticleModifier(arc::variation_application_time::on_update, new arc::particle_lifetime_modifier_2d<Flip<Quadratic>, arc::vec2>(&arc::particle_2d::Scale, arc::vec2::ZeroVector));

	}
	
	void player::Load()
	{
		PlayerSprite.Texture = texture_2d::create("assets/textures/arcShip.png", true);
		PlayerSprite.Transform.Location = { 0.1f, 0.1f};
		PlayerSprite.Transform.Scale = {0.1f, 0.1f};
		PlayerSprite.Transform.ZOrder = 0.2f;

		auto TL = HPR::QUAD::get_corner<TopLeft>(PlayerSprite.get_transform());
		MinDist = vec2::Dist(TL, PlayerSprite.get_location());

		for (uint8_t x = 0; x < 32; x++)
		{
			for (uint8_t y = 0; y < 32; y++)
			{
				if (PlayerSprite.Texture->get_pixel_color({ x, y }).a != 0)
				{
					CollisionData.push_back( std::make_shared<vec2<uint8_t>>( x, y ));
				}
			}
		}

		PlayerSprite.Texture->ClearData();
	}

	void player::Reset()
	{
		PlayerSprite.Transform.Location = { 0.f, 0.f };
		PlayerSprite.Transform.Scale = { 0.1f, 0.1f };
		PlayerSprite.Transform.ZOrder = 0.2f;
		Velocity = vec2::ZeroVector;
	}

	void player::on_update(float delta_time)
	{
		if (auto* SWL = (space_war*)OwningLayer)
		{
			if (SWL && SWL->GameState == game_state::Play)
			{
				float lerpalpha = 0;
				Velocity.x *= 0.97f*delta_time*60.f;
				Velocity.y *= 0.97f*delta_time*60.f;

				if (CInput::is_key_pressed(ARC_KEY_A) || CInput::is_key_pressed(ARC_KEY_W) || CInput::is_key_pressed(ARC_KEY_S) || CInput::is_key_pressed(ARC_KEY_D) || CInput::is_key_pressed(ARC_KEY_RIGHT) || CInput::is_key_pressed(ARC_KEY_LEFT) || CInput::is_key_pressed(ARC_KEY_UP) || CInput::is_key_pressed(ARC_KEY_DOWN)) {
					lerpalpha += delta_time;
					if (CInput::is_key_pressed(ARC_KEY_A) || CInput::is_key_pressed(ARC_KEY_LEFT)) {
						if (CInput::is_key_pressed(ARC_KEY_D) || CInput::is_key_pressed(ARC_KEY_RIGHT)) Velocity.x = 0;
						else Velocity.x = -1;
					}
					else if (CInput::is_key_pressed(ARC_KEY_D) || CInput::is_key_pressed(ARC_KEY_RIGHT)) Velocity.x = 1;
					if (CInput::is_key_pressed(ARC_KEY_W) || CInput::is_key_pressed(ARC_KEY_UP)) {
						if (CInput::is_key_pressed(ARC_KEY_S) || CInput::is_key_pressed(ARC_KEY_DOWN)) Velocity.y = 0;
						else Velocity.y = 1;
					}
					else if (CInput::is_key_pressed(ARC_KEY_S) || CInput::is_key_pressed(ARC_KEY_DOWN)) Velocity.y = -1;

					vec2 emissionPoint = { 0.0f, -0.05f };
					float s = std::sin(PlayerSprite.get_rotation());
					float c = std::cos(PlayerSprite.get_rotation());

					vec2 rotated = vec2(-emissionPoint.y*s, emissionPoint.y * c);
					ParticleSystem.Defaults.Location = PlayerSprite.get_location();
					ParticleSystem.Defaults.Velocity = Math::InterpF<Linear>(ParticleSystem.Defaults.Velocity, (Velocity + rotated)* -1, lerpalpha);
					ParticleSystem.Emit();
				}
				else {
					lerpalpha-=delta_time;
					lerpalpha = std::clamp(lerpalpha, 0.f, 1.f);
				}

				PlayerSprite.Transform.Location.x += Velocity.x * Speed * delta_time;
				PlayerSprite.Transform.Location.y += Velocity.y * Speed * delta_time;

				PlayerSprite.Transform.Location.x = std::clamp<float>(PlayerSprite.Transform.Location.x, -1.6f, 1.6f);
				PlayerSprite.Transform.Location.y = std::clamp<float>(PlayerSprite.Transform.Location.y, -0.94f, 0.94f);
				ParticleSystem.on_update(delta_time);
			}
 			const vec2 mouse_location = CInput::get_mouse_xy();
			auto window_height = application::Get().get_window().get_height();
			auto window_width = application::Get().get_window().get_width();

 			const float y = -((mouse_location.y / window_height)*2-1);
 			const float x = 1.6666f *((mouse_location.x / window_width)*2-1);
 			
			float x_angle = Math::Conv<Radians, Degree>(atan2(
 					y - PlayerSprite.Transform.Location.y,
 					x - PlayerSprite.Transform.Location.x)
 				);
			PlayerSprite.Transform.Rotation = Math::Conv<Degree, Radians>(x_angle - 90.f);
		}
	}

	void player::on_event(event& event)
	{
		event_dispatcher dispatcher(event);
	}
}
