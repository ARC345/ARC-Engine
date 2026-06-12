//--------EntryPoint-----------------
#include "ARC/Core/EntryPoint.h"
//-----------------------------------
#include <ARC.h>
#include "EditorApp.h"
#include "EditorLayer.h"

namespace arc {
	application* create_application()
	{
		return new editor_app();
	}
	editor_app::editor_app() :
		application("ARC-Editor")
	{
		push_layer(new editor_layer());
	}
}