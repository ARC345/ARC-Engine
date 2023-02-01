#pragma once
#include "Component.h"
#include "ARC/Types/Vector.h"
#include "ARC/Types/Delegate.h"
#include "entt.hpp"

namespace ARC {

	using SManager = entt::registry;
	class CEntity;

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
		
		//void OnUpdateEditor(float DeltaTime, CEditorCamera& pCamera);
		void OnUpdateRuntime(float DeltaTime);
		void OnViewportResize(TVec2<uint32_t> pNewSize);
	
		void SerializeToText(const TString& pFilepath);
		void SerializeToBinary(const TString& pFilepath);
		bool DeserializeFromText(const TString& pFilepath);
		bool DeserializeFromBinary(const TString& pFilepath);

		template<typename... Ts>
		decltype(auto) FilterByComponents() { return m_Manager.view<Ts...>(); }

	private:
		void SerializeEntity(YAML::Emitter& pOut, CEntity pEntity);
		void DeserializeEntity(YAML::Emitter& pOut, CEntity pEntity);
	
	private:
		TVec2<uint32_t> m_ViewportSize;
		
		SManager m_Manager;

		TDelegate<bool(const TString&)> m_TreeNodeBeginFunc; 
		TDelegate<void()> m_TreeNodeEndFunc;
	
		friend class CSceneHierarchyPanel;
	};
}