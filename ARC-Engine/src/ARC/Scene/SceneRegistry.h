#pragma once

#include "ARC/Scene/Scene.h"
#include "ARC/Scene/Entity.h"

namespace ARC {

	struct SComponentTraits
	{
		template<typename T>
		static constexpr inline
		bool IsComponent() { return std::is_base_of_v<CComponentBase, T>; }

		template<typename T>
		static constexpr inline
		decltype(auto) GetName()
		{
			static_assert(IsComponent<T>());
			return SHPR::GetClassName<T>();
		}

		template<typename T>
		static inline
		auto& GetFlags()
		{
			static_assert(IsComponent<T>());
			return T::Flags;
		}

		template<typename T>
		static constexpr const inline
		auto GetID() noexcept {
			static const auto value = ItrlCounter();
			return value;
		}

	private:
		static inline
			auto ItrlCounter() noexcept {
			static TUInt32 value = 0;
			return value++;
		}
	};

	struct SSceneRegistry
	{
		struct SMetaComponent {
			TDelegate<void(CEntity pEntity)> RemoveComponent;
			TDelegate<CComponentBase* (CEntity pEntity)> GetComponent;
			TDelegate<CComponentBase* (CEntity pEntity)> AddComponent;
			TDelegate<CComponentBase* (CEntity pDstEntity, const CEntity pSrcEntity)> CopyComponent;
			TDelegate<TUInt32& ()> GetFlags;
			TDelegate<TString ()> GetName;
		};

		template<typename T>
		static void SetupComponent()
		{
			static bool bRet = false;
			if(bRet) return;
			bRet = true;

			const TString& compName = SComponentTraits::GetName<T>();
			const auto compID = SComponentTraits::GetID<T>();
			mRegisteredComponentNames.push_back(compName);
			mRegisteredComponentNameIDMap[compName] = compID;
			
			auto& mc = mMetaComponents.emplace(compID, SMetaComponent()).first->second;
			mc.GetComponent = [](CEntity pEntity) -> CComponentBase* {
				static_assert(SComponentTraits::IsComponent<T>());
				if (pEntity && pEntity.HasComponent<T>())
					return &pEntity.GetComponent<T>();
				return nullptr;
					};
			mc.AddComponent = [](CEntity pEntity) -> CComponentBase* {
				static_assert(SComponentTraits::IsComponent<T>());
				if (pEntity && !pEntity.HasComponent<T>())
					return &pEntity.AddComponent<T>();
				return nullptr;
				};
			mc.RemoveComponent = [](CEntity pEntity) {
				static_assert(SComponentTraits::IsComponent<T>());
				if (pEntity && pEntity.HasComponent<T>())
					pEntity.RemoveComponent<T>();
				};
			mc.CopyComponent = [](CEntity pDstEntity, const CEntity pSrcEntity) -> CComponentBase* {
				static_assert(SComponentTraits::IsComponent<T>());
				if (pDstEntity && pSrcEntity && pSrcEntity.HasComponent<T>())
					return &pDstEntity.AddComponent<T>(pSrcEntity.GetComponent<T>());
				return nullptr;
				};
			mc.GetFlags = []() -> TUInt32& {
				static_assert(SComponentTraits::IsComponent<T>());
				return SComponentTraits::GetFlags<T>();
				};
			mc.GetName = []() {
				static_assert(SComponentTraits::IsComponent<T>());
				return SComponentTraits::GetName<T>();
				};
		};

		static auto& GetRegisteredComponentsNames() { return mRegisteredComponentNames; }
		static auto& GetRegisteredComponentNameIDMap() { return mRegisteredComponentNameIDMap; }
		static auto& GetMetaComponents() { return mMetaComponents; }

	private:
		// @Editor functionality @ TODO wrap around macro
	
		static inline std::vector<TString> mRegisteredComponentNames;
		static inline std::unordered_map<TString, TUInt32> mRegisteredComponentNameIDMap;
		static inline std::unordered_map<TUInt32, SMetaComponent> mMetaComponents;

	};

}