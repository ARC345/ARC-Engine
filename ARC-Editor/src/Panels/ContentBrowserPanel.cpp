#include "PCH\arc_pch.h"
#include "imgui\imgui.h"
#include "ContentBrowserPanel.h"
#include "ARC\Renderer\Texture.h"
#include <corecrt_wstring.h>

static const std::filesystem::path sAssetsDirectoryPath = "assets";

namespace arc {
	content_browser_panel::content_browser_panel() :
		current_dir_(sAssetsDirectoryPath)
	{
		directory_icon_ = texture_2d::create("resources/icons/directory icon.png");
		file_icon_ = texture_2d::create("resources/icons/file icon.png");
	}
	void content_browser_panel::on_imgui_render()
	{
		ImGui::begin("Content Browser Panel");

		if (current_dir_ != sAssetsDirectoryPath) {
			if (ImGui::Button("<-"))
				current_dir_ = current_dir_.parent_path();
		}
		
		static auto padding = 16.0f;
		static auto thumbnailSize = 120.0f;
		auto panelWidth = ImGui::get_content_region_avail().x;
		int columnCount = int(panelWidth/(padding+thumbnailSize));
		
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f,0.17f,0.16f,1.f));
		ImGui::Columns(math::Max(columnCount, 1), 0, false);
		
		for (auto& i : std::filesystem::directory_iterator(current_dir_))
		{
			const auto& path = i.path();
			auto relativePath = std::filesystem::relative(path, sAssetsDirectoryPath);
			auto filename = relativePath.filename().string();

			ImGui::PushID(filename.c_str());
			auto icon = i.is_directory() ? directory_icon_ : file_icon_;
			
			ImGui::ImageButton((ImTextureID)(u64)icon->get_renderer_id(), ImVec2(thumbnailSize, thumbnailSize), {0,1}, {1,0});

			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* relativePathCStr = relativePath.c_str();
				ImGui::set_drag_drop_payload("CONTENT_BROWSER_ITEM", relativePathCStr, (std::wcslen(relativePathCStr) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}
			
			if (ImGui::is_item_hovered() && ImGui::is_mouse_double_clicked(ImGuiMouseButton_Left)) {
				if (i.is_directory())
					current_dir_ /= path.filename();
				else
					{}
			}
			ImGui::TextWrapped(filename.c_str());
			ImGui::NextColumn();
			ImGui::PopID();
		}
		ImGui::Columns(1);
		ImGui::SliderFloat("Thumbnail size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 64);
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
		ImGui::end();
	}
}