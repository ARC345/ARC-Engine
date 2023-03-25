#pragma once
#include <filesystem>
#include "ARC/Types/Pointer.h"

namespace ARC { class CTexture2D; }

namespace ARC {
	class CContentBrowserPanel
	{
	public:
		CContentBrowserPanel();

		void OnImGuiRender();

	private:
		std::filesystem::path mCurrentDir;
		TRef<CTexture2D> mDirectoryIcon;
		TRef<CTexture2D> mFileIcon;
	};
}