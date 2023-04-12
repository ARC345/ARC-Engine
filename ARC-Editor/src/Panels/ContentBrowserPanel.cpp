#include "PCH\arc_pch.h"
#include "imgui\imgui.h"
#include "ContentBrowserPanel.h"
#include "ARC\Renderer\Texture.h"
#include <corecrt_wstring.h>

static const std::filesystem::path sAssetsDirectoryPath = "assets";

namespace ARC {
	CContentBrowserPanel::CContentBrowserPanel() :
		mCurrentDir(sAssetsDirectoryPath)
	{
		mDirectoryIcon = CTexture2D::Create("resources/icons/directory icon.png");
		mFileIcon = CTexture2D::Create("resources/icons/file icon.png");
	}
	void CContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser Panel");

		if (mCurrentDir != sAssetsDirectoryPath) {
			if (ImGui::Button("<-"))
				mCurrentDir = mCurrentDir.parent_path();
		}
		
		static auto padding = 16.0f;
		static auto thumbnailSize = 120.0f;
		auto panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = int(panelWidth/(padding+thumbnailSize));
		
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f,0.17f,0.16f,1.f));
		ImGui::Columns(SMath::Max(columnCount, 1), 0, false);
		
		for (auto& i : std::filesystem::directory_iterator(mCurrentDir))
		{
			const auto& path = i.path();
			auto relativePath = std::filesystem::relative(path, sAssetsDirectoryPath);
			auto filename = relativePath.filename().string();

			ImGui::PushID(filename.c_str());
			auto icon = i.is_directory() ? mDirectoryIcon : mFileIcon;
			
			ImGui::ImageButton((ImTextureID)(TUInt64)icon->GetRendererID(), ImVec2(thumbnailSize, thumbnailSize), {0,1}, {1,0});

			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* relativePathCStr = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", relativePathCStr, (std::wcslen(relativePathCStr) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}
			
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if (i.is_directory())
					mCurrentDir /= path.filename();
				else
					{}
			}
			ImGui::TextWrapped(filename.c_str());
			ImGui::NextColumn();
			ImGui::PopID();
		}
		ImGui::Columns(1);
		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 64);
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
		ImGui::End();
	}
}