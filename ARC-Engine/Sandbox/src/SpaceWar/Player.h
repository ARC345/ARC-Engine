#pragma once
#include "ARC\Types\Transform2D.h"
#include "ARC\Objects\Primitive2D.h"
#include <vector>
#include <memory>

namespace ARC { class CLayer; }

namespace ARC { class CKeyReleasedEvent; }

namespace ARC { class CKeyPressedEvent; }

namespace ARC { class CEvent; }
namespace ARC { class CMouseMovedEvent; }

namespace ARC {
	class CPlayer
	{
	public:
		CPlayer();
		void Load();
		void Reset();
		void OnUpdate(float _DeltaTime);
		void OnEvent(ARC::CEvent& _Event);
		CPrimitive2D PlayerSprite;
		uint8_t bIsAlive : 1;
		float Speed;
		// Minimum distance from object for collision to activate
		float MinDist;
		FVec2 Velocity;
		CLayer* OwningLayer;
		std::vector<std::shared_ptr<TVec2<uint8_t>>> CollisionData;
	protected:
	private:
	};
}