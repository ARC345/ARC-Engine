#pragma once
#include "ARC/Core/Application.h"

namespace ARC {
	class CEditorApp : public ARC::CApplication
	{
	public:
		CEditorApp();
		~CEditorApp() {};

		inline virtual std::string GetAppName() override { return "ARC-Editor"; };
	};
}