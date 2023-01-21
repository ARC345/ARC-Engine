#include "arc_pch.h"
#include "Entity.h"
#include "Scene.h"

namespace ARC
{

	CEntity::CEntity(TEntityID pHandle, CScene* pScene) : m_Entity(pHandle), m_Scene(pScene)
	{
	}

	void CEntity::OnKill()
	{
		m_Entity = entt::null;
		m_Scene = nullptr;
	}

	bool CEntity::IsValid() const
	{
		return m_Entity != entt::null && m_Scene != nullptr;
	}

	void CEntity::Serialize(const bool pbBinary, YAML::Emitter& pEmitter)
	{
		ARC_CORE_ASSERT(IsValid(), "Cannot serialize a dead entity")

		Serializer(pEmitter);
	}
}