#pragma once
#include "ARC\Objects\Primitive2D.h"
#include "ARC\Types\vector.h"
#include <array>
#include <vector>
#include <memory>
namespace ARC {
	class CRock
	{
	public:
		void Load(FVec2 _Velocity, FVec2 _Position);
		void OnUpdate(float _DeltaTime);

		void CalculateRandomVelocity();
		void CalculateRandomPosition();

		CPrimitive2D Sprite;
		FVec2 Velocity;
		int SpriteIndex = 0;
		float MinDist = 0;
		float RotationVelocity = 0;
	private:
		static constexpr size_t s_NumberOfRockSprites = 5;
	public:
		static std::array<std::vector<std::shared_ptr<TVec2<uint8_t>>>, s_NumberOfRockSprites> SpriteCollisionData;
	};
}