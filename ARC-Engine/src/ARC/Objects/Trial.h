#pragma once

#include "Ecs.h"
#include "ARC/Types/Vector.h"
#include "ARC/Renderer/Layer.h"
#include "ARC/Objects/Primitive2D.h"

namespace ARC {
struct CPosition { FVec2 value; };
struct CVelocity { FVec2 value; };
struct CAcceleration { FVec2 value; };
struct CLife { float value; };
struct CHealth { float value; };
struct CRender
{
	CPrimitive2D Primitive;

	CRender()
	{
		Primitive.Color = FColor::Blue;
		Primitive.Transform.Scale = FVec2{ 0.8f, 0.5f };
	}
};
struct CHitbox
{
	CPrimitive2D Primitive;

	CHitbox()
	{
		Primitive.Color = FColor::Red;
		Primitive.Transform.Scale = FVec2{ 0.8f, 0.5f };
	}
};

struct CCooldown
{
	float value, maxValue;
};

struct TPlayer
{
};
struct TBoss
{
};
struct TBullet
{
};
struct TPlayerBullet
{
};
RegisterComponent(CPosition);
RegisterComponent(CVelocity);
RegisterComponent(CAcceleration);
RegisterComponent(CRender);
RegisterComponent(CHealth);
RegisterComponent(CLife);
RegisterComponent(CHitbox);
RegisterComponent(CCooldown);
}
