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
		m_ControlledEntity = pEntity;
	}

	CEntity* CEntityController::GetControlledEntity() {	return m_ControlledEntity; }

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

		if (CInput::IsKeyPressed(ARC_KEY_W))
			trans.Location.y -= speed * pDeltaTime;
		if (CInput::IsKeyPressed(ARC_KEY_A))
			trans.Location.x += speed * pDeltaTime;
		if (CInput::IsKeyPressed(ARC_KEY_S))
			trans.Location.y += speed * pDeltaTime;
		if (CInput::IsKeyPressed(ARC_KEY_D))
			trans.Location.x -= speed * pDeltaTime;
	}

}
