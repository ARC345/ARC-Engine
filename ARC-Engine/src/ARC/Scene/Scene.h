#pragma once
#include "ARC\Objects\Ecs.h"
#include "Entity.h"
#include "Component.h"
#include "Tag.h"
#include "ARC\Types\Vector.h"

namespace ARC {
	class CScene
	{
		using MyComponents = ECS::IMPL::ITRL_ComponentList<HPR::CustomCounter<ECS::ComponentCounterId>::Next() - 1>::type;
		using MySignatures = ECS::IMPL::ITRL_SignatureList<HPR::CustomCounter<ECS::SignatureCounterId>::Next() - 1>::type;
		using MyTags = ECS::IMPL::ITRL_TagList<HPR::CustomCounter<ECS::TagCounterId>::Next() - 1>::type;
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

	private:
		TVec2<uint32_t> m_ViewportSize;

		MyManager m_Manager;

		friend class CSceneHierarchyPanel;
	};
}