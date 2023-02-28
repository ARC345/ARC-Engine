#pragma once

#include "ARC/Scene/Scene.h"
#include "ARC/Scene/Entity.h"

namespace ARC {
	struct SSceneRegistry
	{
		template<typename T>
		static void SetupComponent(uint32_t pOverrideFlags = T::Flags)
		{
			const TString& compName = CComponentTraits::GetName<T>();
			mRegisteredComponentNames.push_back(compName);

			if (pOverrideFlags & ECF::ShowInPropertiesPanel) {
				mTypeErasedGetComponentFuncs[compName].Bind([](CEntity pEntity) -> CComponentBase* {
					static_assert(CComponentTraits::IsComponent<T>());
					if (pEntity && pEntity.HasComponent<T>())
						return &pEntity.GetComponent<T>();
					return nullptr;
					});
				mTypeErasedAddComponentFuncs[compName].Bind([](CEntity pEntity) -> CComponentBase* {
					static_assert(CComponentTraits::IsComponent<T>());
					if (pEntity && !pEntity.HasComponent<T>())
						return &pEntity.AddComponent<T>();
					return nullptr;
					});
				mTypeErasedRemoveComponentFuncs[compName].Bind([](CEntity pEntity) {
					static_assert(CComponentTraits::IsComponent<T>());
					if (pEntity && pEntity.HasComponent<T>())
						return pEntity.RemoveComponent<T>();
					});
			}
		};

		static auto& GetRegisteredComponentsNames() { return mRegisteredComponentNames; }
		static auto& GetTypeErasedGetComponentFuncs() { return mTypeErasedGetComponentFuncs; }
		static auto& GetTypeErasedAddComponentFuncs() { return mTypeErasedAddComponentFuncs; }
		static auto& GetTypeErasedRemoveComponentFuncs() { return mTypeErasedRemoveComponentFuncs; }

	private:
		// @Editor functionality @ TODO wrap around macro
		static inline std::vector<TString> mRegisteredComponentNames;
		static inline std::unordered_map<TString, TDelegate<void(CEntity pEntity)>> mTypeErasedRemoveComponentFuncs;
		static inline std::unordered_map<TString, TDelegate<CComponentBase* (CEntity pEntity)>> mTypeErasedGetComponentFuncs;
		static inline std::unordered_map<TString, TDelegate<CComponentBase* (CEntity pEntity)>> mTypeErasedAddComponentFuncs;
	};
}