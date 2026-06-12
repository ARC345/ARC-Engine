#pragma once
#include "ARC/Types/Transform2D.h"
#include "ARC/Objects/Primitive2D.h"
#include <vector>
#include <memory>
#include "ARC/Renderer/ParticleSystem2D.h"

namespace arc { class layer; }

namespace arc { class key_released_event; }

namespace arc { class key_pressed_event; }

namespace arc { class event; }
namespace arc { class mouse_moved_event; }

namespace arc {
	class player
	{
	public:
		player();
		void Load();
		void Reset();
		void on_update(float delta_time);
		void on_event(arc::event& event);
		primitive_2d PlayerSprite;
		particle_system_2d ParticleSystem;
		uint8_t bIsAlive : 1;
		float Speed;
		// Minimum distance from object for collision to activate
		float MinDist;
		vec2 Velocity;
		layer* OwningLayer;
		std::vector<std::shared_ptr<vec2<uint8_t>>> CollisionData;
	protected:
	private:
	};
}