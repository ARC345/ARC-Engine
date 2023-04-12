#pragma once
#include "Component.h"
#include "BasicComponents.h"
#include "entt.hpp"
#include "Scene.h"
#include "ARC/Core/UUID.h"

namespace ARC { class CScene; }
namespace ARC { class CEntity; }

namespace ARC {
	using TEntityID = entt::entity;
	
	class CEntity
	{
	public:
		CEntity() =default;
		CEntity(TEntityID pHandle, CScene* pScene) : mEntity(pHandle), mScene(pScene) {};
		CEntity(const CEntity&) = default;

		TEntityID GetID() const { return mEntity; }
		const TString& GetName() { return GetComponent<CNameComponent>().Name; }
		TUUID GetUUID() { return GetComponent<CIDComponent>().ID; }

		bool IsValid() const
		{
			return mEntity != entt::null && mScene != nullptr;
		}
		template<typename T>
		void RemoveComponent() {
			ARC_CORE_ASSERT(mScene && HasComponent<T>())
			mScene->GetManager().remove<T>(mEntity);
		}
		template<typename T>
		[[nodiscard]] inline bool HasComponent() const { 
			ARC_CORE_ASSERT(mScene);
			return mScene && mScene->GetManager().all_of<T>(mEntity);
		}
		template<typename T>
		[[nodiscard]] inline T& GetComponent() {
			ARC_CORE_ASSERT(mScene && HasComponent<T>());
			auto& rval = mScene->GetManager().get<T>(mEntity);
			return rval;
		}
		template<typename T>
		[[nodiscard]] inline T& GetComponent() const {
			ARC_CORE_ASSERT(mScene && HasComponent<T>());
			auto& rval = mScene->GetManager().get<T>(mEntity);
			return rval;
		}
		template<typename T, typename ... Ts>
		inline T& AddComponent(Ts&&... pArgs) {
			ARC_CORE_ASSERT(mScene)
			auto& comp = mScene->GetManager().emplace<T>(mEntity, std::forward<Ts>(pArgs)...);
			comp.OnConstruct(*this);

			return comp;
		}
		
		CEntity Duplicate()
		{
			return mScene->DuplicateEntity(*this);
		}

		operator bool() const { return mEntity != entt::null; }
		bool operator == (const CEntity _) const { return mEntity == _.mEntity && mScene == _.mScene; }
		bool operator != (const CEntity _) const { return !(*this == _); }
	protected:
		virtual void OnKill()
		{
			mEntity = entt::null;
			mScene = nullptr;
		}
	private:
		TEntityID mEntity = entt::null;
		CScene* mScene = nullptr;


		friend class CScene;
	};
}