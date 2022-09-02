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
		m_SelectedEntity = {};
	}
	
	void CSceneHierarchyPanel::OnImGuiRender()
	{
		static bool bDraw = true;
		ImGui::ShowDemoWindow(&bDraw);
		
		ImGui::Begin("Scene Hierarchy");
		
		m_Context->GetManager().UpdateEntities([&](auto pID) {
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

					if (!m_SelectedEntity.HasComponent<tComponentType>() && ImGui::MenuItem(CComponentTraits::GetName<tComponentType>()))
					{
						m_SelectedEntity.AddComponent<tComponentType>();
						ImGui::CloseCurrentPopup();
					}
				});
				ImGui::EndPopup();
			}
		}
		ImGui::End();

// 		ImGui::Begin("Style");
// 		auto& colors = ImGui::GetStyle().Colors;
// 
// 		#define ColorStuff(xx) ImGui::ColorEdit4(#xx, &colors[xx].x) 
// 		
// 		ColorStuff(ImGuiCol_WindowBg);			
// 		ColorStuff(ImGuiCol_PopupBg);			
// 		ColorStuff(ImGuiCol_Border);				
// 		ColorStuff(ImGuiCol_FrameBg);			
// 		ColorStuff(ImGuiCol_FrameBgHovered);		
// 		ColorStuff(ImGuiCol_FrameBgActive);		
// 		ColorStuff(ImGuiCol_TitleBg);			
// 		ColorStuff(ImGuiCol_TitleBgActive);		
// 		ColorStuff(ImGuiCol_MenuBarBg);			
// 		ColorStuff(ImGuiCol_ScrollbarBg);		
// 		ColorStuff(ImGuiCol_ScrollbarGrab);		
// 		ColorStuff(ImGuiCol_ScrollbarGrabHovered);
// 		ColorStuff(ImGuiCol_ScrollbarGrabActive);
// 		ColorStuff(ImGuiCol_Header);				
// 		ColorStuff(ImGuiCol_HeaderHovered);		
// 		ColorStuff(ImGuiCol_HeaderActive);		
// 		ColorStuff(ImGuiCol_Separator);			
// 		ColorStuff(ImGuiCol_SeparatorHovered);	
// 		ColorStuff(ImGuiCol_SeparatorActive);	
// 		ColorStuff(ImGuiCol_ResizeGrip);			
// 		ColorStuff(ImGuiCol_ResizeGripHovered);	
// 		ColorStuff(ImGuiCol_ResizeGripActive);	
// 		ColorStuff(ImGuiCol_Tab);				
// 		ColorStuff(ImGuiCol_TabHovered);			
// 		ColorStuff(ImGuiCol_TabActive);			
// 		ColorStuff(ImGuiCol_TabUnfocused);		
// 		ColorStuff(ImGuiCol_TabUnfocusedActive);	
// 		ColorStuff(ImGuiCol_DockingPreview);		
// 		ColorStuff(ImGuiCol_DockingEmptyBg);		
// 		ColorStuff(ImGuiCol_PlotLines);			
// 		ColorStuff(ImGuiCol_PlotLinesHovered);	
// 		ColorStuff(ImGuiCol_PlotHistogram);		
// 		ColorStuff(ImGuiCol_PlotHistogramHovered);
// 		ColorStuff(ImGuiCol_TableHeaderBg);		
// 		ColorStuff(ImGuiCol_TableBorderStrong);	
// 		ColorStuff(ImGuiCol_TableBorderLight);	
// 		ColorStuff(ImGuiCol_TableRowBg);			
// 		ColorStuff(ImGuiCol_TableRowBgAlt);		
// 		ColorStuff(ImGuiCol_TextSelectedBg);		
// 		ColorStuff(ImGuiCol_DragDropTarget);		
// 		ColorStuff(ImGuiCol_NavHighlight);		
// 		ColorStuff(ImGuiCol_NavWindowingHighlight);
// 		ColorStuff(ImGuiCol_NavWindowingDimBg);	
// 		ColorStuff(ImGuiCol_ModalWindowDimBg);	
// 		ImGui::End();
	}

	void CSceneHierarchyPanel::DrawEntityNode(CEntity pEntity)
	{
		if (!pEntity.HasComponent<CNameComponent>()) return;

		auto& name = pEntity.GetComponent<CNameComponent>();

		{
			ImGuiTreeNodeFlags TreeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((m_SelectedEntity == pEntity) ? ImGuiTreeNodeFlags_Selected : 0);
			bool bOpened = ImGui::TreeNodeEx((void*)(uint32_t)pEntity.GetID(), TreeFlags, name.Name.CStr());

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
			if (pEntity.HasComponent<tComponentType>())
			{
				if (tComponentType::Flags & ECF::ShowInPropertiesPanel)
				{
					if(tComponentType::Flags & ECF::ShowHeader)
					{
						TString name = CComponentTraits::GetName<tComponentType>();
						bool bTreeNodeOpen = ImGui::TreeNodeEx((void*)typeid(tComponentType).hash_code(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth, name.CStr());
						bool bDeleteComp = false;
						if (ImGui::BeginPopupContextItem())
						{
							if (ImGui::MenuItem("Delete Component"))
								bDeleteComp = true;
							ImGui::EndPopup();
						}

						if (bTreeNodeOpen)
						{
							pEntity.GetComponent<tComponentType>().DrawPropertiesUI(pEntity);
							ImGui::TreePop();
						}
						if (bDeleteComp)
							pEntity.RemoveComponent<tComponentType>();
					}
					else
					{
						pEntity.GetComponent<tComponentType>().DrawPropertiesUI(pEntity);
					}
				}
			}
		});
	}
}