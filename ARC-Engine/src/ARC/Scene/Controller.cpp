#include "arc_pch.h"
#include "Controller.h"
#include "Component.h"
#include "Entity.h"
#include "ARC\Input\Input.h"
#include "ARC\Input\KeyCodes.h"
#include "Scene.h"
#include "BasicComponents.h"

namespace ARC
{
	void CEntityController::Setup(CEntity* pEntity)
	{
		mControlledEntity = pEntity;
	}

	CEntity* CEntityController::GetControlledEntity() {	return mControlledEntity; }

	//----------------------------------------------------------------------------//

	void CCameraController::OnCreate()
	{

	}

	void CCameraController::OnDestroy()
	{

	}
	
	void CCameraController::OnUpdate(float pDeltaTime)
	{
		return;
		if (!GetControlledEntity()->IsValid()) return;
			
		auto& trans = GetControlledEntity()->GetComponent<CTransform2DComponent>().Transform;
		float speed = 5.f;

		if (SInput::IsKeyPressed(EKey::W))
			trans.Location.y -= speed * pDeltaTime;
		if (SInput::IsKeyPressed(EKey::A))
			trans.Location.x += speed * pDeltaTime;
		if (SInput::IsKeyPressed(EKey::S))
			trans.Location.y += speed * pDeltaTime;
		if (SInput::IsKeyPressed(EKey::D))
			trans.Location.x -= speed * pDeltaTime;
	}

}
