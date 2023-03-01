#include "arc_pch.h"
#include "Entity.h"
#include "Scene.h"

namespace ARC
{

	CEntity::CEntity(TEntityID pHandle, CScene* pScene) : mEntity(pHandle), mScene(pScene)
	{
	}

	void CEntity::OnKill()
	{
		mEntity = entt::null;
		mScene = nullptr;
	}

	bool CEntity::IsValid() const
	{
		return mEntity != entt::null && mScene != nullptr;
	}
}