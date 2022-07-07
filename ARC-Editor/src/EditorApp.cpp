//--------EntryPoint-----------------
#include "ARC/Core/EntryPoint.h"
//-----------------------------------
#include <ARC.h>
#include "EditorApp.h"
#include "EditorLayer.h"
#include "Atom/Atom.h"

namespace ARC {
	CApplication* CreateApplication()
	{
		return new CEditorApp();
	}
	CEditorApp::CEditorApp() :
		CApplication("ARC-Editor")
	{
		PushLayer(new CEditorLayer());
	}
}