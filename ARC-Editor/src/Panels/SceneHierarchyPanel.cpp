#include "PCH\arc_pch.h"
#include "SceneHierarchyPanel.h"

#include "imgui\imgui.h"
#include "ARC\Scene\Scene.h"
#include "ARC\Scene\SceneCamera.h"
#include "Utils\MPL\Interface.hpp"
#include "ARC\Scene\Component.h"
#include "ARC\Scene\Entity.h"
#include "ARC\Scene\BasicComponents.h"
#include "ARC\Scene\SceneRegistry.h"

namespace ARC {
	CSceneHierarchyPanel::CSceneHierarchyPanel(const TRef<CScene>& pScene)
	{
		SetContext(pScene);
	}

	void CSceneHierarchyPanel::SetContext(const TRef<CScene>& pScene)
	{
		mContext = pScene;
		mSelectedEntity = {};
	}
	
	void CSceneHierarchyPanel::OnImGuiRender()
	{
		static bool bDraw = true;
		ImGui::ShowDemoWindow(&bDraw);
		
		ImGui::Begin("Scene Hierarchy");
		
		mContext->GetManager().each([&](auto pID) {
			DrawEntityNode(CEntity(pID, mContext.get()));
			});


		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			mSelectedEntity = {};

		// right click on a blank space
		if (ImGui::BeginPopupContextWindow(nullptr, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				mContext->CreateEntity("Empty Entity");
			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		
		if(mSelectedEntity)
		{
			if (CComponentBase* comp = SSceneRegistry::GetTypeErasedGetComponentFuncs()[CComponentTraits::GetName<CNameComponent>()](mSelectedEntity)) {
				ImGui::Text("Name:");
				ImGui::SameLine(100.f, 20.f);
				comp->DrawPropertiesUI(mSelectedEntity);
			}

			bool bNameCompFound = false;

			for(auto& _ : SSceneRegistry::GetTypeErasedGetComponentFuncs()) {
				if (auto* comp = _.second(mSelectedEntity)) {
					bool bDeleteComponent = false;

					if (!bNameCompFound && _.first == CComponentTraits::GetName<CNameComponent>()) {
						bNameCompFound = true;
						continue;
					}

					if (comp->GetFlags() & ECF::EComponentFlags::ShowInPropertiesPanel
						&& ImGui::TreeNodeEx(_.first.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth, _.first.c_str())) {
						if (ImGui::BeginPopupContextItem())
						{
							if (ImGui::MenuItem("Delete Component"))
								bDeleteComponent = true;
							ImGui::EndPopup();
						}
						comp->DrawPropertiesUI(mSelectedEntity);

						ImGui::TreePop();
					}
					if (bDeleteComponent)
						SSceneRegistry::GetTypeErasedRemoveComponentFuncs()[_.first](mSelectedEntity);
				}
			}

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent"))
			{
				for(auto& comp : SSceneRegistry::GetTypeErasedGetComponentFuncs())
				{
					if (!comp.second(mSelectedEntity) && ImGui::MenuItem(comp.first.c_str()))
					{
						SSceneRegistry::GetTypeErasedAddComponentFuncs()[comp.first](mSelectedEntity);
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

		ImGuiTreeNodeFlags TreeFlags = 
			ImGuiTreeNodeFlags_OpenOnArrow
			| ImGuiTreeNodeFlags_SpanAvailWidth
			| ImGuiTreeNodeFlags_OpenOnDoubleClick
			| ((mSelectedEntity == pEntity) ? ImGuiTreeNodeFlags_Selected : 0);
		bool bOpened = ImGui::TreeNodeEx((void*)(uint32_t)pEntity.GetID(), TreeFlags, name.Name.c_str());

		if (ImGui::IsItemClicked())
			mSelectedEntity = pEntity;

		bool bDeleteEntity = false;

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				bDeleteEntity = true;
			ImGui::EndPopup();
		}

		if (bOpened)
			ImGui::TreePop();
		if (bDeleteEntity)
		{
			mContext->RemoveEntity(pEntity);
			mSelectedEntity = {};
		}
	}
}