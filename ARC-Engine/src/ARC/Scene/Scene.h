#pragma once
#include "ARC/Objects/Ecs.h"
#include "Entity.h"
#include "Component.h"
#include "Tag.h"
#include "ARC/Types/Vector.h"
#include "ARC/Types/String.h"
#include "ARC/Core/Yaml.h"

namespace ARC {
	class CScene
	{
		using MyComponents = AUTO_COMPONENTS();
		using MySignatures = AUTO_SIGNATURES();
		using MyTags = AUTO_TAGS();
		using MyManager = ECS::CManager<ECS::SSettings<MyComponents, MyTags, MySignatures>>;
	public:
		CScene();
		~CScene();
		
		void SetupComponents();

		CEntity CreateEntity(const TString& pName = "Entity");
		void RemoveEntity(CEntity Entity);

		inline size_t GetEntityCount() const { return m_Manager.GetEntityCount(); }

		inline MyManager& GetManager() { return m_Manager; }
		inline const MyManager& GetManager() const { return m_Manager; }
		
		void OnUpdate(float DeltaTime);
		void OnViewportResize(TVec2<uint32_t> pNewSize);

		void SerializeToText(const TString& pFilepath);
		void SerializeToBinary(const TString& pFilepath);
		bool DeserializeFromText(const TString& pFilepath);
		bool DeserializeFromBinary(const TString& pFilepath);

	private:
		static void SerializeEntity(YAML::Emitter& pOut, CEntity pEntity);
		static void DeserializeEntity(YAML::Emitter& pOut, CEntity pEntity);

	private:
		TVec2<uint32_t> m_ViewportSize;

		MyManager m_Manager;

		friend class CSceneHierarchyPanel;
	};
}