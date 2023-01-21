#include "PCH\arc_pch.h"
#include "SceneHierarchyPanel.h"

#include "imgui\imgui.h"
#include "ARC\Scene\Scene.h"
#include "ARC\Scene\SceneCamera.h"
#include "Utils\MPL\Interface.hpp"
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
		
		m_Context->GetManager().each([&](auto pID) {
			DrawEntityNode(CEntity(pID, m_Context.get()));
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectedEntity = {};

		// right click on a blank space
		if (ImGui::BeginPopupContextWindow(nullptr, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");
			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		
		if(m_SelectedEntity)
		{
			for(auto& _ : m_Context->GetTypeErasedGetComponentFuncs()) {
				auto* comp = _.second(m_SelectedEntity);
				if (comp && ImGui::TreeNodeEx(_.first.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth, _.first.c_str())) {
					comp->DrawPropertiesUI(m_SelectedEntity);

					ImGui::TreePop();
				}
			}

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent"))
			{
				for(auto& comp : m_Context->GetTypeErasedGetComponentFuncs())
				{
					if (!comp.second(m_SelectedEntity) && ImGui::MenuItem(comp.first.c_str()))
					{
						m_Context->GetTypeErasedAddComponentFuncs()[comp.first](m_SelectedEntity);
						ImGui::CloseCurrentPopup();
					}
				}
				ImGui::EndPopup();
			}
		}
		ImGui::End();

 		ImGui::Begin("Style");
 		auto& colors = ImGui::GetStyle().Colors;
 
 		#define ColorStuff(xx) ImGui::ColorEdit4(#xx, &colors[xx].x) 
 		
 		ColorStuff(ImGuiCol_WindowBg);			
 		ColorStuff(ImGuiCol_PopupBg);			
 		ColorStuff(ImGuiCol_Border);				
 		ColorStuff(ImGuiCol_FrameBg);			
 		ColorStuff(ImGuiCol_FrameBgHovered);		
 		ColorStuff(ImGuiCol_FrameBgActive);		
 		ColorStuff(ImGuiCol_TitleBg);			
 		ColorStuff(ImGuiCol_TitleBgActive);		
 		ColorStuff(ImGuiCol_MenuBarBg);			
 		ColorStuff(ImGuiCol_ScrollbarBg);		
 		ColorStuff(ImGuiCol_ScrollbarGrab);		
 		ColorStuff(ImGuiCol_ScrollbarGrabHovered);
 		ColorStuff(ImGuiCol_ScrollbarGrabActive);
 		ColorStuff(ImGuiCol_Header);				
 		ColorStuff(ImGuiCol_HeaderHovered);		
 		ColorStuff(ImGuiCol_HeaderActive);		
 		ColorStuff(ImGuiCol_Separator);			
 		ColorStuff(ImGuiCol_SeparatorHovered);	
 		ColorStuff(ImGuiCol_SeparatorActive);	
 		ColorStuff(ImGuiCol_ResizeGrip);			
 		ColorStuff(ImGuiCol_ResizeGripHovered);	
 		ColorStuff(ImGuiCol_ResizeGripActive);	
 		ColorStuff(ImGuiCol_Tab);				
 		ColorStuff(ImGuiCol_TabHovered);			
 		ColorStuff(ImGuiCol_TabActive);			
 		ColorStuff(ImGuiCol_TabUnfocused);		
 		ColorStuff(ImGuiCol_TabUnfocusedActive);	
 		ColorStuff(ImGuiCol_DockingPreview);		
 		ColorStuff(ImGuiCol_DockingEmptyBg);		
 		ColorStuff(ImGuiCol_PlotLines);			
 		ColorStuff(ImGuiCol_PlotLinesHovered);	
 		ColorStuff(ImGuiCol_PlotHistogram);		
 		ColorStuff(ImGuiCol_PlotHistogramHovered);
 		ColorStuff(ImGuiCol_TableHeaderBg);		
 		ColorStuff(ImGuiCol_TableBorderStrong);	
 		ColorStuff(ImGuiCol_TableBorderLight);	
 		ColorStuff(ImGuiCol_TableRowBg);			
 		ColorStuff(ImGuiCol_TableRowBgAlt);		
 		ColorStuff(ImGuiCol_TextSelectedBg);		
 		ColorStuff(ImGuiCol_DragDropTarget);		
 		ColorStuff(ImGuiCol_NavHighlight);		
 		ColorStuff(ImGuiCol_NavWindowingHighlight);
 		ColorStuff(ImGuiCol_NavWindowingDimBg);	
 		ColorStuff(ImGuiCol_ModalWindowDimBg);
		
		#undef ColorStuff
 		ImGui::End();
	}

	void CSceneHierarchyPanel::DrawEntityNode(CEntity pEntity)
	{
		if (!pEntity.HasComponent<CNameComponent>()) return;

		auto& name = pEntity.GetComponent<CNameComponent>();

		{
			ImGuiTreeNodeFlags TreeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((m_SelectedEntity == pEntity) ? ImGuiTreeNodeFlags_Selected : 0);
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
}