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

		CEntity GetSelectedEntity() const { return mSelectedEntity; }
		void SetSelectedEntity(CEntity p) { mSelectedEntity = p; }

	private:
		void DrawEntityNode(CEntity pEntity);

	private:
		TRef<CScene> mContext;
		CEntity mSelectedEntity;
	};
}