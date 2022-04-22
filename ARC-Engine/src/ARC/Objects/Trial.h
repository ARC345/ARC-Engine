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
		Primitive.Color = CColor::Blue;
		Primitive.Transform.Scale = FVec2{ 0.8f, 0.5f };
	}
};
struct CHitbox
{
	CPrimitive2D Primitive;

	CHitbox()
	{
		Primitive.Color = CColor::Red;
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
Define_Component(CPosition);
Define_Component(CVelocity);
Define_Component(CAcceleration);
Define_Component(CRender);
Define_Component(CHealth);
Define_Component(CLife);
Define_Component(CHitbox);
Define_Component(CCooldown);
}
