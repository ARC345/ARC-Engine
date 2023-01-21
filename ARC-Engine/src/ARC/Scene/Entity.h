#pragma once
#include "Component.h"
#include "entt.hpp"

namespace ARC { class CScene; }
namespace ARC { class CEntity; }
	

namespace ARC::ITRL::META {
	template < class TComponent >
	void GetComponent(
		CEntity pEntity,
		CComponentBase*& pComp
	) {
		ARC_CORE_INFO("Hi");
		pComp = pEntity.HasComponent<TComponent>() ? &pEntity.GetComponent<TComponent>() : nullptr;
	}
	template < class TComponent >
	auto& AssignComponentFromMetaAny(
		CEntity pEntity,
		YAML::Emitter& pNode
	) {
		return pEntity.AddComponent<TComponent>();
	}
	template<typename TComponent, typename ... Ts>
	void AddComponent(
		CEntity pEntity,
		CComponentBase* pOut,
		Ts&&... pArgs
	) {
		pOut = &pEntity.AddComponent<TComponent>();
	}


	template<typename TComponent>
	void RemoveComponent(
		CEntity pEntity
	) {
		pEntity.RemoveComponent<TComponent>();
	}
}

namespace ARC {
	using TEntityID = entt::entity;
	
	class CEntity
	{
	public:
		CEntity() =default;
		CEntity(TEntityID pHandle, CScene* pScene);
		CEntity(const CEntity&) = default;

		TEntityID GetID() const { return m_Entity; }
		bool IsValid() const;

		void Serialize(const bool pbBinary, YAML::Emitter& pEmitter);
		void Deserialize(const bool pbBinary, YAML::Emitter& pEmitter);
// 
// 		template<typename T>
// 		void Serializer() {
// 			pOut << YAML::Key << HPR::GetClassName<T>();
// 			pOut << YAML::Value << YAML::BeginMap;
// 			pOut << YAML::EndMap;
// 		}
		template<typename T>
		void RemoveComponent() {
			ARC_CORE_ASSERT(m_Scene && HasComponent<T>())
				m_Scene->GetManager().remove<T>(m_Entity);
		}
		template<typename T>
		[[nodiscard]] inline bool HasComponent() const { 
			ARC_CORE_ASSERT(m_Scene);
			return m_Scene && m_Scene->GetManager().all_of<T>(m_Entity);
		}
		template<typename T>
		[[nodiscard]] inline T& GetComponent() {
			ARC_CORE_ASSERT(m_Scene && HasComponent<T>());
			auto& rval = m_Scene->GetManager().get<T>(m_Entity);
			return rval;
		}
		template<typename T, typename ... Ts>
		inline T& AddComponent(Ts&&... pArgs) {
			ARC_CORE_ASSERT(m_Scene)
			auto& comp = m_Scene->GetManager().emplace<T>(m_Entity, std::forward<Ts>(pArgs)...);
			comp.OnConstruct(this);

			return comp;
		}
		
		operator bool() const { return m_Entity != entt::null; }
		bool operator == (const CEntity _) const { return m_Entity == _.m_Entity && m_Scene == _.m_Scene; }
		bool operator != (const CEntity _) const { return !(*this == _); }
		
		TMulticastDelegate<void(YAML::Emitter&)> Serializer;
	protected:
		virtual void OnKill();

	private:
		TEntityID m_Entity = entt::null;
		CScene* m_Scene = nullptr;


		friend class CScene;
	};
}