#pragma once
#include "ARC\Objects\Primitive2D.h"
#include "ARC\Types\vector.h"
#include <array>
#include <vector>
#include <memory>
namespace arc {
	class rock
	{
	public:
		void Load(vec2 velocity, vec2 position);
		void on_update(float delta_time);

		void calculate_random_velocity();
		void calculate_random_position();

		primitive_2d Sprite;
		vec2 Velocity;
		int SpriteIndex = 0;
		float MinDist = 0;
		float RotationVelocity = 0;
	private:
		static constexpr size_t number_of_rock_sprites = 5;
	public:
		static std::array<std::vector<std::shared_ptr<vec2<uint8_t>>>, number_of_rock_sprites> SpriteCollisionData;
	};
}