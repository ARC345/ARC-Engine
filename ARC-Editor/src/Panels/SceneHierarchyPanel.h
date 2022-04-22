#pragma once

namespace ARC {
	class CScene{
		friend class CSceneHierarchyPanel;
	};

	class CSceneHierarchyPanel
	{
	public:
		CSceneHierarchyPanel()=default;
		CSceneHierarchyPanel(const TRef<CScene>& pScene);

		void SetContext(const TRef<CScene>& pScene);
		void OnImGuiRender();

	private:
		//void DrawEntityNode();

	private:
		TRef<CScene> m_Context;
	};
}