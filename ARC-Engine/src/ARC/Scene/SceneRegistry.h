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
			m_RegisteredComponentNames.push_back(compName);

			if (pOverrideFlags & ECF::ShowInPropertiesPanel) {
				m_TypeErasedGetComponentFuncs[compName].Bind([](CEntity pEntity) -> CComponentBase* {
					static_assert(CComponentTraits::IsComponent<T>());
					if (pEntity && pEntity.HasComponent<T>())
						return &pEntity.GetComponent<T>();
					return nullptr;
					});
				m_TypeErasedAddComponentFuncs[compName].Bind([](CEntity pEntity) -> CComponentBase* {
					static_assert(CComponentTraits::IsComponent<T>());
					if (pEntity && !pEntity.HasComponent<T>())
						return &pEntity.AddComponent<T>();
					return nullptr;
					});
				m_TypeErasedRemoveComponentFuncs[compName].Bind([](CEntity pEntity) {
					static_assert(CComponentTraits::IsComponent<T>());
					if (pEntity && pEntity.HasComponent<T>())
						return pEntity.RemoveComponent<T>();
					});
			}
		};

		static auto& GetRegisteredComponentsNames() { return m_RegisteredComponentNames; }
		static auto& GetTypeErasedGetComponentFuncs() { return m_TypeErasedGetComponentFuncs; }
		static auto& GetTypeErasedAddComponentFuncs() { return m_TypeErasedAddComponentFuncs; }
		static auto& GetTypeErasedRemoveComponentFuncs() { return m_TypeErasedRemoveComponentFuncs; }

	private:
		// @Editor functionality @ TODO wrap around macro
		static inline std::vector<TString> m_RegisteredComponentNames;
		static inline std::unordered_map<TString, TDelegate<void(CEntity pEntity)>> m_TypeErasedRemoveComponentFuncs;
		static inline std::unordered_map<TString, TDelegate<CComponentBase* (CEntity pEntity)>> m_TypeErasedGetComponentFuncs;
		static inline std::unordered_map<TString, TDelegate<CComponentBase* (CEntity pEntity)>> m_TypeErasedAddComponentFuncs;
	};
}