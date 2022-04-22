#include "PCH\arc_pch.h"

namespace ARC {
	CSceneHierarchyPanel::CSceneHierarchyPanel(const TRef<CScene>& pScene)
	{
		SetContext(pScene);
	}

	void CSceneHierarchyPanel::SetContext(const TRef<CScene>& pScene)
	{
		m_Context = pScene;
	}
	
	void CSceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		ImGui::End();
	}
}