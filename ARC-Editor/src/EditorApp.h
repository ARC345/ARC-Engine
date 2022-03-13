#pragma once
#include "ARC/Core/Application.h"

namespace ARC {
	class CEditorApp : public ARC::Core::CApplication
	{
	public:
		CEditorApp();
		~CEditorApp() {};

		inline virtual std::string GetAppName() override { return "ARC-Editor"; };
	};
}