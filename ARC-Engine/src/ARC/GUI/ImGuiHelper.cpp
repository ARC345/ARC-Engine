#include "arc_pch.h"
#include "ImGuiHelper.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace ARC {

	void SGuiHelper::DrawGuiControl(FVec1& pVec, const char* pID, float pColumnWidth, const FVec1& pDefaults)
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
			pVec.x = 0;
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##X", &pVec.x, 0.1f);
		ImGui::PopStyleColor(3);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::PopID();
		ImGui::Columns(1);
	}

	void SGuiHelper::DrawGuiControl(FVec2& pVec, const char* pID, float pColumnWidth, const FVec2& pDefaults)
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
			pVec.x = pDefaults.x;
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##X", &pVec.x, 0.1f);
		ImGui::PopStyleColor(3);
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.38f, 0.65f, 0.2f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.55f, 0.85f, 0.35f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.38f, 0.75f, 0.2f, 1.0f });

		ImGui::PushFont(BoldFont);
		if (ImGui::Button("Y"))
			pVec.y = pDefaults.y;
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &pVec.y, 0.1f);
		ImGui::PopStyleColor(3);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::PopID();
		ImGui::Columns(1);
	}

	void SGuiHelper::DrawGuiControl(FVec3& pVec, const char* pID, float pColumnWidth, const FVec3& pDefaults)
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
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
		ImVec2 buttonSize = { lineHeight + 1.f, lineHeight };
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f, 0.1f, 0.15f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.8f, 0.1f, 0.15f, 1.0f });

		ImGui::PushFont(BoldFont);
		if (ImGui::Button("X"))
			pVec.x = pDefaults.x;
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##X", &pVec.x, 0.1f);
		ImGui::PopStyleColor(3);
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.38f, 0.65f, 0.2f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.55f, 0.85f, 0.35f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.38f, 0.75f, 0.2f, 1.0f });

		ImGui::PushFont(BoldFont);
		if (ImGui::Button("Y"))
			pVec.y = pDefaults.y;
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &pVec.y, 0.1f);
		ImGui::PopStyleColor(3);
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.1f, 0.2f, 1.f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.3f, 0.4f, 1.f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.1f, 0.2f, 1.f, 1.f });

		ImGui::PushFont(BoldFont);
		if (ImGui::Button("Z"))
			pVec.z = pDefaults.z;
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &pVec.z, 0.1f);
		ImGui::PopStyleColor(3);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::PopID();
		ImGui::Columns(1);
	}
	
	void SGuiHelper::DrawGuiControl(FVec4 & pVec, const char* pID, float pColumnWidth, const FVec4& pDefaults)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto BoldFont = io.Fonts->Fonts[0];

		ImGui::PushID(pID);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, pColumnWidth);
		ImGui::Spacing();
		ImGui::Text(pID);
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
		ImVec2 buttonSize = { lineHeight + 1.f, lineHeight };
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f, 0.1f, 0.15f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.8f, 0.1f, 0.15f, 1.0f });

		ImGui::PushFont(BoldFont);
		if (ImGui::Button("X"))
			pVec.x = pDefaults.x;
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##X", &pVec.x, 0.1f);
		ImGui::PopStyleColor(3);
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.38f, 0.65f, 0.2f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.55f, 0.85f, 0.35f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.38f, 0.75f, 0.2f, 1.0f });

		ImGui::PushFont(BoldFont);
		if (ImGui::Button("Y"))
			pVec.y = pDefaults.y;
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &pVec.y, 0.1f);
		ImGui::PopStyleColor(3);
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.1f, 0.2f, 1.f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.3f, 0.4f, 1.f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.1f, 0.2f, 1.f, 1.f });

		ImGui::PushFont(BoldFont);
		if (ImGui::Button("Z"))
			pVec.z = pDefaults.z;
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &pVec.z, 0.1f);
		ImGui::PopStyleColor(3);
		ImGui::PopItemWidth();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f, 0.8f, 0.8f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.9f, 0.9f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.6f, 0.6f, 0.6f, 1.f });

		ImGui::SameLine();
		ImGui::PushFont(BoldFont);
		if (ImGui::Button("W"))
			pVec.w = pDefaults.w;
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##W", &pVec.w, 0.1f);
		ImGui::PopStyleColor(3);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::PopID();
		ImGui::Columns(1);
	}

	void SGuiHelper::DrawGuiControl(FColor4& pColor, const char* pID, float pColumnWidth, const FColor4& pDefaults /*= FColor4::White()*/)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto BoldFont = io.Fonts->Fonts[0];

		ImGui::PushID(pID);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, pColumnWidth);
		ImGui::Spacing();
		ImGui::Text(pID);
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
		ImVec2 buttonSize = { lineHeight + 1.f, lineHeight };
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f, 0.1f, 0.15f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.8f, 0.1f, 0.15f, 1.0f });

		ImGui::PushFont(BoldFont);
		if (ImGui::Button("R"))
			pColor.x = pDefaults.x;
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##R", &pColor.x, 0.1f);
		ImGui::PopStyleColor(3);
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.38f, 0.65f, 0.2f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.55f, 0.85f, 0.35f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.38f, 0.75f, 0.2f, 1.0f });

		ImGui::PushFont(BoldFont);
		if (ImGui::Button("G"))
			pColor.y = pDefaults.y;
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##G", &pColor.y, 0.1f);
		ImGui::PopStyleColor(3);
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.1f, 0.2f, 1.f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.3f, 0.4f, 1.f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.1f, 0.2f, 1.f, 1.f });

		ImGui::PushFont(BoldFont);
		if (ImGui::Button("B"))
			pColor.z = pDefaults.z;
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##B", &pColor.z, 0.1f);
		ImGui::PopStyleColor(3);
		ImGui::PopItemWidth();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f, 0.8f, 0.8f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.9f, 0.9f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.6f, 0.6f, 0.6f, 1.f });

		ImGui::SameLine();
		ImGui::PushFont(BoldFont);
		if (ImGui::Button("A"))
			pColor.w = pDefaults.w;
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##A", &pColor.w, 0.1f);
		ImGui::PopStyleColor(3);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::PopID();
		ImGui::Columns(1);

		ImGui::SameLine();
		ImGui::ColorEdit4("##ColorPicker", &pColor.x, ImGuiColorEditFlags_NoInputs);
	}
}