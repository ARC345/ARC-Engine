#pragma once
#include <filesystem>
#include "ARC/Types/Pointer.h"

namespace arc { class texture_2d; }

namespace arc {
	class content_browser_panel
	{
	public:
		content_browser_panel();

		void on_imgui_render();

	private:
		std::filesystem::path current_dir_;
		ref<texture_2d> directory_icon_;
		ref<texture_2d> file_icon_;
	};
}