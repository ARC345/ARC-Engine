#include "arc_pch.h"
#include "Entity.h"
#include "Tag.h"
#include "Scene.h"

namespace ARC
{

	CEntity::CEntity(EntityID pHandle, CScene* pScene) : m_Entity(pHandle), m_Scene(pScene)
	{

	}

	void CEntity::OnKill()
	{
		m_Entity = ECS::InvalidEntityID;
		m_Scene = nullptr;
	}

	bool CEntity::IsValid() const
	{
		return m_Entity != ECS::InvalidEntityID && m_Scene != nullptr;
	}
}