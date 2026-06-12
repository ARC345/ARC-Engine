#include "arc_pch.h"
#include "Controller.h"
#include "Component.h"
#include "Entity.h"
#include "ARC\Input\Input.h"
#include "ARC\Input\KeyCodes.h"
#include "Scene.h"
#include "BasicComponents.h"

namespace arc
{
	void entity_controller::set_up(entity* pEntity)
	{
		controlled_entity_ = pEntity;
	}

	entity* entity_controller::get_controlled_entity() {	return controlled_entity_; }

	//----------------------------------------------------------------------------//

	void camera_controller::on_create()
	{

	}

	void camera_controller::OnDestroy()
	{

	}
	
	void camera_controller::on_update(float pDeltaTime)
	{
		return;
		if (!get_controlled_entity()->is_valid()) return;
			
		auto& trans = get_controlled_entity()->get_component<transform_2d_component>().Transform;
		float speed = 5.f;

		if (input::is_key_pressed(key::W))
			trans.Location.y -= speed * pDeltaTime;
		if (input::is_key_pressed(key::A))
			trans.Location.x += speed * pDeltaTime;
		if (input::is_key_pressed(key::S))
			trans.Location.y += speed * pDeltaTime;
		if (input::is_key_pressed(key::D))
			trans.Location.x -= speed * pDeltaTime;
	}

}
