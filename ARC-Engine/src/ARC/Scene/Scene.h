#pragma once
#include "Component.h"
#include "ARC/Types/Vector.h"
#include "ARC/Types/Delegate.h"
#include "entt.hpp"

namespace ARC {

	using SManager = entt::registry;

	class CEditorCamera;

	class CEntity;

	class CScene
	{
	public:
		CScene();
		~CScene();
			
		CEntity CreateEntity(const TString& pName = "Entity");
		void RemoveEntity(CEntity Entity);
	
		inline size_t GetEntityCount() const { return mManager.alive(); }
	
		inline SManager& GetManager() { return mManager; }
		inline const SManager& GetManager() const { return mManager; }
		
		void OnUpdateEditor(float DeltaTime, CEditorCamera& pCamera);
		void OnUpdateRuntime(float DeltaTime);
		void OnViewportResize(TVec2<uint32_t> pNewSize);
	
		void SerializeToText(const TString& pFilepath);
		void SerializeToBinary(const TString& pFilepath);
		bool DeserializeFromText(const TString& pFilepath);
		bool DeserializeFromBinary(const TString& pFilepath);

		template<typename... Ts>
		decltype(auto) FilterByComponents() { return mManager.view<Ts...>(); }

	private:
		void SerializeEntity(YAML::Emitter& pOut, CEntity pEntity);
		void DeserializeEntity(YAML::Emitter& pOut, CEntity pEntity);
	
	private:
		TVec2<uint32_t> mViewportSize;
		
		SManager mManager;

		TDelegate<bool(const TString&)> mTreeNodeBeginFunc; 
		TDelegate<void()> mTreeNodeEndFunc;
		
		friend class CSceneHierarchyPanel;
	};
}