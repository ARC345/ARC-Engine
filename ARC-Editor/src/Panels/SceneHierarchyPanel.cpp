#include "PCH\arc_pch.h"
#include "SceneHierarchyPanel.h"

#include "imgui\imgui.h"
#include "ARC\Scene\Scene.h"
#include "ARC\Scene\SceneCamera.h"
#include "Utils\MPL\Interface.hpp"
#include <tuple>
#include "ARC\Scene\Component.h"

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
		static bool bDraw = true;
		ImGui::ShowDemoWindow(&bDraw);
		
		ImGui::Begin("Scene Hierarchy");
		
		m_Context->GetManager().UpdateEntities([&](auto pID){
			DrawEntityNode(CEntity(pID, m_Context.get()));
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectedEntity = {};

		ImGui::End();

		ImGui::Begin("Properties");
		
		if(m_SelectedEntity)
			DrawComponents(m_SelectedEntity);
		
		ImGui::End();
	}

	void CSceneHierarchyPanel::DrawEntityNode(CEntity pEntity)
	{
		if (!pEntity.HasComponent<CNameComponent>()) return;

		auto& name = pEntity.GetComponent<CNameComponent>();

		{
			ImGuiTreeNodeFlags TreeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((m_SelectedEntity == pEntity) ? ImGuiTreeNodeFlags_Selected : 0);
			bool bOpened = ImGui::TreeNodeEx((void*)(uint32_t)pEntity.GetID(), TreeFlags, name.Name.c_str());

			if (ImGui::IsItemClicked())
			{
				m_SelectedEntity = pEntity; 
			}
			if (bOpened)
			{
				ImGui::TreePop();
			}
		}
	}

	void CSceneHierarchyPanel::DrawComponents(CEntity pEntity)
	{
		MPL::forTypes<CScene::MyComponents>([&](auto t){
			if (pEntity.HasComponent<ECS_TYPE(t)>())
			{
				if (ECS_TYPE(t)::Flags & CComponentBase::ShowInPropertiesPanel)
				{
					pEntity.GetComponent<ECS_TYPE(t)>().DrawPropertiesUI(pEntity);
				}
			}
		});
	}
}