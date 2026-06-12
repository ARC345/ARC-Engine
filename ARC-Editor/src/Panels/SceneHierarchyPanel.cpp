#include "PCH\arc_pch.h"
#include "SceneHierarchyPanel.h"

#include "imgui\imgui.h"
#include "ARC\Scene\Scene.h"
#include "ARC\Scene\SceneCamera.h"
#include "ARC\Scene\Component.h"
#include "ARC\Scene\Entity.h"
#include "ARC\Scene\BasicComponents.h"
#include "ARC\Scene\SceneRegistry.h"

namespace arc {
	scene_hierarchy_panel::scene_hierarchy_panel(const ref<scene>& pScene)
	{
		set_context(pScene);
	}

	void scene_hierarchy_panel::set_context(const ref<scene>& pScene)
	{
		context_ = pScene;
		selected_entity_ = {};
	}
	void scene_hierarchy_panel::on_imgui_render()
	{
		static bool bDraw = true;
		ImGui::ShowDemoWindow(&bDraw);
		
		ImGui::begin("Scene Hierarchy");
		
		context_->get_manager().each([&](auto pID) {
			DrawEntityNode(entity(pID, context_.get()));
			});


		if (ImGui::is_mouse_down(0) && ImGui::is_window_hovered())
			selected_entity_ = {};

		// right click on a blank space
		if (ImGui::BeginPopupContextWindow(nullptr))
		{
			if (ImGui::MenuItem("create Empty Entity"))
				context_->create_entity("Empty Entity");
			ImGui::EndPopup();
		}

		ImGui::end();

		ImGui::begin("Properties");
		
		if(selected_entity_)
		{
			if (component_base* comp = scene_registry::get_meta_components()[component_traits::get_id<name_component>()].get_component(selected_entity_)) {
				ImGui::Text("Name:");
				ImGui::SameLine(100.f, 20.f);
				comp->draw_properties_ui(selected_entity_);
			}

			bool bNameCompFound = false;

			for(auto& _ : scene_registry::get_meta_components()) {
				if (auto* comp = _.second.get_component(selected_entity_)) {
					bool bDeleteComponent = false;

					if (!bNameCompFound && _.first == component_traits::get_id<name_component>()) {
						bNameCompFound = true;
						continue;
					}

					if (comp->get_flags() & ecf::component_flags::ShowInPropertiesPanel
						&& ImGui::TreeNodeEx(_.second.get_name().c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth, _.second.get_name().c_str())) {
						if (ImGui::BeginPopupContextItem())
						{
							if (ImGui::MenuItem("Delete Component"))
								bDeleteComponent = true;
							ImGui::EndPopup();
						}
						comp->draw_properties_ui(selected_entity_);

						ImGui::TreePop();
					}
					if (bDeleteComponent)
						_.second.remove_component(selected_entity_);
				}
			}

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("add_component");

			if (ImGui::BeginPopup("add_component"))
			{
				for(auto& compMeta : scene_registry::get_meta_components())
				{
					if (!compMeta.second.get_component(selected_entity_) && ImGui::MenuItem(compMeta.second.get_name().c_str()))
					{
						compMeta.second.add_component(selected_entity_);
						ImGui::CloseCurrentPopup();
					}
				}
				ImGui::EndPopup();
			}

		}
		ImGui::end();

 		ImGui::begin("Style");
 		auto& colors = ImGui::get_style().Colors;
 
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
 		ImGui::end();
	}

	void scene_hierarchy_panel::DrawEntityNode(entity pEntity)
	{
		if (!pEntity.has_component<name_component>()) return;

		auto& name = pEntity.get_component<name_component>();

		ImGuiTreeNodeFlags TreeFlags = 
			ImGuiTreeNodeFlags_OpenOnArrow
			| ImGuiTreeNodeFlags_SpanAvailWidth
			| ImGuiTreeNodeFlags_OpenOnDoubleClick
			| ((selected_entity_ == pEntity) ? ImGuiTreeNodeFlags_Selected : 0);
		bool bOpened = ImGui::TreeNodeEx((void*)(uint64_t)pEntity.get_id(), TreeFlags, name.Name.c_str());

		if (ImGui::is_item_clicked())
			selected_entity_ = pEntity;

		bool bDeleteEntity = false;

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				bDeleteEntity = true;
			if (ImGui::MenuItem("Duplicate Entity"))
				selected_entity_ = selected_entity_.Duplicate();
			ImGui::EndPopup();
		}

		if (bOpened)
			ImGui::TreePop();
		if (bDeleteEntity)
		{
			context_->remove_entity(pEntity);
			selected_entity_ = {};
		}
	}
}