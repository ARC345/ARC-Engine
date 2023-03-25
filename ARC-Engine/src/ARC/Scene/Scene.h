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

		void HandleCircleXCircleCollision();
		void HandleCircleXPolyCollision();
		void HandlePolyXPolyCollision();

		void OnViewportResize(TVec2<uint32_t> pNewSize);
	
		void SerializeToText(const std::filesystem::path& pFilepath);
		void SerializeToBinary(const std::filesystem::path& pFilepath);
		bool DeserializeFromText(const std::filesystem::path& pFilepath);
		bool DeserializeFromBinary(const std::filesystem::path& pFilepath);

		template<typename... Ts>
		decltype(auto) FilterByComponents() { return mManager.view<Ts...>(); }

		inline decltype(auto) GetViewportSize() { return mViewportSize; }
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