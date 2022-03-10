//--------EntryPoint-----------------
#include "ARC/Core/EntryPoint.h"
//-----------------------------------
#include <ARC.h>
#include "EditorApp.h"
#include "EditorLayer.h"

namespace ARC {
	Core::CApplication* Core::CreateApplication()
	{
		return new CEditorApp();
	}
	CEditorApp::CEditorApp() :
		CApplication("ARC-Editor")
	{
		PushLayer(new CEditorLayer());
	}
}