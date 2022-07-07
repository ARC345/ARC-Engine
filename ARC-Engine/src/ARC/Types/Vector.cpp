#include "arc_pch.h"
#include "Vector.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace ARC
{
	void FVec1::DrawGuiControl(const char* ID, float pColumnWidth)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto BoldFont = io.Fonts->Fonts[0];

		ImGui::PushID(ID);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, pColumnWidth);
		ImGui::Text(ID);
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
		ImVec2 buttonSize = { lineHeight + 1.f, lineHeight };
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f, 0.1f, 0.15f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.8f, 0.1f, 0.15f, 1.0f });

		ImGui::PushFont(BoldFont);
		if (ImGui::Button("X"))
			x = 0;
		ImGui::PopFont();
		
		ImGui::SameLine();
		ImGui::DragFloat("##X", &x, 0.1f);
		ImGui::PopStyleColor(3);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::PopID();
		ImGui::Columns(1);
	}

	void FVec2::DrawGuiControl(const char* pID, float pColumnWidth, type pDefaults)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto BoldFont = io.Fonts->Fonts[0];

		ImGui::PushID(pID);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, pColumnWidth);
		ImGui::Text(pID);
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
		ImVec2 buttonSize = { lineHeight + 1.f, lineHeight };
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f, 0.1f, 0.15f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.8f, 0.1f, 0.15f, 1.0f });

		ImGui::PushFont(BoldFont);
		if (ImGui::Button("X"))
			x = pDefaults.x;
		ImGui::PopFont();
		
		ImGui::SameLine();
		ImGui::DragFloat("##X", &x, 0.1f);
		ImGui::PopStyleColor(3);
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.38f, 0.65f, 0.2f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.55f, 0.85f, 0.35f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.38f, 0.75f, 0.2f, 1.0f });
		
		ImGui::PushFont(BoldFont);
		if (ImGui::Button("Y"))
			y = pDefaults.y;
		ImGui::PopFont();
		
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &y, 0.1f);
		ImGui::PopStyleColor(3);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::PopID();
		ImGui::Columns(1);
	}

	void FVec3::DrawGuiControl(const char* pID, float pColumnWidth, type pDefaults)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto BoldFont = io.Fonts->Fonts[0];

		ImGui::PushID(pID);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, pColumnWidth);
		ImGui::Spacing();
		ImGui::Text(pID);
		ImGui::NextColumn();
		
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
		ImVec2 buttonSize = { lineHeight+1.f, lineHeight };
		ImGui::PushStyleColor(ImGuiCol_Button, {0.8f, 0.1f, 0.15f, 1.f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.9f, 0.2f, 0.2f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.8f, 0.1f, 0.15f, 1.0f});

		ImGui::PushFont(BoldFont);
		if (ImGui::Button("X"))
			x = pDefaults.x;
		ImGui::PopFont();
		
		ImGui::SameLine();
		ImGui::DragFloat("##X", &x, 0.1f);
		ImGui::PopStyleColor(3);
		ImGui::PopItemWidth();
		
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.38f, 0.65f, 0.2f, 1.f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.55f, 0.85f, 0.35f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.38f, 0.75f, 0.2f, 1.0f});

		ImGui::PushFont(BoldFont);
		if (ImGui::Button("Y"))
			y = pDefaults.y;
		ImGui::PopFont();
		
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &y, 0.1f);
		ImGui::PopStyleColor(3);
		ImGui::PopItemWidth();
		
		ImGui::SameLine();
		
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.1f, 0.2f, 1.f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.3f, 0.4f, 1.f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.1f, 0.2f, 1.f, 1.f });

		ImGui::PushFont(BoldFont);
		if (ImGui::Button("Z"))
			z = pDefaults.z;
		ImGui::PopFont();
		
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &z, 0.1f);
		ImGui::PopStyleColor(3);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::PopID();
		ImGui::Columns(1);
	}

	template <typename T /*= float*/>
	void TVec3<T>::DrawGui(float pColumnWidth) const
	{

	}
}