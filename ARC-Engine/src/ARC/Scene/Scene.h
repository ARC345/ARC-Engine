#pragma once
#include "Entity.h"
#include "ComponentTest.h"
#include "ARC/Types/Vector.h"
#include "ARC/Types/Delegate.h"
#include "ARC/Core/Yaml.h"
#include "entt.hpp"
#include <functional>

namespace ARC {

	using SManager = entt::registry;
	
	class CScene
	{
	public:
		CScene();
		~CScene();
			
		CEntity CreateEntity(const TString& pName = "Entity");
		void RemoveEntity(CEntity Entity);
	
		inline size_t GetEntityCount() const { return m_Manager.alive(); }
	
		inline SManager& GetManager() { return m_Manager; }
		inline const SManager& GetManager() const { return m_Manager; }
		
		void OnUpdate(float DeltaTime);
		void OnViewportResize(TVec2<uint32_t> pNewSize);
	
		void SerializeToText(const TString& pFilepath);
		void SerializeToBinary(const TString& pFilepath);
		bool DeserializeFromText(const TString& pFilepath);
		bool DeserializeFromBinary(const TString& pFilepath);

		template<typename T>
		void SetupComponent(uint32_t pOverrideFlags = T::Flags) {
			const TString& compName = CComponentTraits::GetName<T>();
			m_RegisteredComponentNames.push_back(compName);
			
			if (pOverrideFlags & ECF::ShowInPropertiesPanel) {
				m_TypeErasedGetComponentFuncs[compName].Bind([](CEntity pEntity) -> CComponentBase* {
					static_assert(CComponentTraits::IsComponent<T>());
					if(pEntity && pEntity.HasComponent<T>())
						return &pEntity.GetComponent<T>();
					return nullptr;
				});
				m_TypeErasedAddComponentFuncs[compName].Bind([](CEntity pEntity) -> CComponentBase* {
					static_assert(CComponentTraits::IsComponent<T>());
					if (pEntity && !pEntity.HasComponent<T>())
						return &pEntity.AddComponent<T>();
					return nullptr;
				});
			}
		};

		template<typename... Ts>
		decltype(auto) FilterByComponents() { return m_Manager.view<Ts...>(); }
		decltype(auto) GetRegisteredComponentsNames() { return m_RegisteredComponentNames; }
		auto& GetTypeErasedGetComponentFuncs() { return m_TypeErasedGetComponentFuncs; }
		auto& GetTypeErasedAddComponentFuncs() { return m_TypeErasedAddComponentFuncs; }

		
	private:
		static void SerializeEntity(YAML::Emitter& pOut, CEntity pEntity);
		static void DeserializeEntity(YAML::Emitter& pOut, CEntity pEntity);
	
	private:
		TVec2<uint32_t> m_ViewportSize;
		std::vector<TString> m_RegisteredComponentNames;
		
		SManager m_Manager;

		// @Editor functionality @ TODO wrap around macro
		std::unordered_map<TString, TDelegate<CComponentBase* (CEntity pEntity)>> m_TypeErasedGetComponentFuncs;
		std::unordered_map<TString, TDelegate<CComponentBase* (CEntity pEntity)>> m_TypeErasedAddComponentFuncs;

		TDelegate<bool(const TString&)> m_TreeNodeBeginFunc; 
		TDelegate<void()> m_TreeNodeEndFunc;
	
		friend class CSceneHierarchyPanel;
	};
}