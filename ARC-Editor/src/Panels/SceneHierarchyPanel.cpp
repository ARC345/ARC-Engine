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

		// right click on a blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");
			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		
		if(m_SelectedEntity)
		{
			DrawComponents(m_SelectedEntity);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent"))
			{
				MPL::forTypes<CScene::MyComponents>([&](auto t) {
					using tComponentType = ECS_TYPE(t);

					if (ImGui::MenuItem(CComponentHelper::GetName<tComponentType>()))
					{
						m_SelectedEntity.AddComponent<tComponentType>();
						ImGui::CloseCurrentPopup();
					}
				});
				ImGui::EndPopup();
			}
		}
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

			bool bDeleteEntity = false;

			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Delete Entity"))
					bDeleteEntity = true;
				ImGui::EndPopup();
			}

			if (bOpened)
			{
				ImGui::TreePop();
			}
			if (bDeleteEntity)
			{
				m_Context->RemoveEntity(pEntity);
				m_SelectedEntity = {};
			}
		}
	}

	void CSceneHierarchyPanel::DrawComponents(CEntity pEntity)
	{
		MPL::forTypes<CScene::MyComponents>([&](auto t){
			using tComponentType = ECS_TYPE(t);
			if (pEntity.HasComponent<ECS_TYPE(t)>())
			{
				if (tComponentType::Flags & CComponentBase::ShowInPropertiesPanel)
				{
					std::string name = CComponentHelper::GetName<tComponentType>();
					if(ImGui::TreeNodeEx((void*)typeid(tComponentType).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, name.c_str()))
					{
						pEntity.GetComponent<tComponentType>().DrawPropertiesUI(pEntity);

						ImGui::TreePop();
					}
				}
			}
		});
	}
}