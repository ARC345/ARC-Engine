#pragma once
#include "ARC\Types\Pointer.h"
#include "ARC\Scene\Entity.h"

namespace ARC { class CScene; }

namespace ARC {
	class CSceneHierarchyPanel
	{
	public:
		CSceneHierarchyPanel()=default;
		CSceneHierarchyPanel(const TRef<CScene>& pScene);

		void SetContext(const TRef<CScene>& pScene);
		void OnImGuiRender();

		CEntity GetSelectedEntity() const { return m_SelectedEntity; }

	private:
		void DrawEntityNode(CEntity pEntity);
		void DrawComponents(CEntity pEntity);
		template<typename T>
		void DrawComponent(CEntity& pEntity);
	private:
		TRef<CScene> m_Context;
		CEntity m_SelectedEntity;
	};
}