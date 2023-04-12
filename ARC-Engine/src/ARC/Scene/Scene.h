#pragma once
#include "Component.h"
#include "ARC/Types/Vector.h"
#include "ARC/Types/Delegate.h"
#include "entt.hpp"
#include "ARC/Core/UUID.h"

class b2World;

namespace ARC {

	using SManager = entt::registry;

	class CEditorCamera;
	class CEntity;

	class CScene
	{
	public:
		CScene();
		~CScene();
		
		static TRef<CScene> Copy(const TRef<CScene> p);

		CEntity CreateEntity(const TString& pName = "Entity", const TUUID pUUID = SUUID::Generate());
		void RemoveEntity(CEntity Entity);
	
		inline size_t GetEntityCount() const { return mManager.alive(); }
	
		inline SManager& GetManager() { return mManager; }
		inline const SManager& GetManager() const { return mManager; }
		
		void OnRuntimeBegin();
		void OnRuntimeEnd(); // report cause
		
		void OnUpdateEditor(float DeltaTime, CEditorCamera& pCamera);
		void OnUpdateRuntime(float DeltaTime);

		void OnViewportResize(TVec2<uint32_t> pNewSize);
	
		void SerializeToText(const std::filesystem::path& pFilepath);
		void SerializeToBinary(const std::filesystem::path& pFilepath);
		bool DeserializeFromText(const std::filesystem::path& pFilepath);
		bool DeserializeFromBinary(const std::filesystem::path& pFilepath);

		template<typename... Ts>
		decltype(auto) FilterByComponents() { return mManager.view<Ts...>(); }

		inline decltype(auto) GetViewportSize() { return mViewportSize; }

		CEntity DuplicateEntity(CEntity pSrcEntity);

	private:
		void SerializeEntity(YAML::Emitter& pOut, CEntity pEntity);
		void DeserializeEntity(YAML::Emitter& pOut, CEntity pEntity);
	public:
		TDelegate<void()> dOnRuntimeBegin;
		TDelegate<void()> dOnRuntimeEnd;

	private:
		TVec2<uint32_t> mViewportSize;
		
		SManager mManager;
		b2World* mWorld;

		TDelegate<bool(const TString&)> mTreeNodeBeginFunc;
		TDelegate<void()> mTreeNodeEndFunc;
		
		friend class CSceneHierarchyPanel;
	};
}