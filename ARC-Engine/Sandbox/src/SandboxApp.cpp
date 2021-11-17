#include <ARC.h>
#include "ARC\Events\Event.h"
#include "SandboxApp.h"
#include "ARC\GUI\ImGuiLayer.h"
#include "ARC\Helpers\Helpers.h"

ARC::Core::CApplication* ARC::Core::CreateApplication()
{
	return new CSandbox;
}


//-------------------[Layer]----------------------//
void ExampleLayer::OnUpdate()
{
}

void ExampleLayer::OnEvent(ARC::CEvent& _event)
{
}
//-------------------[Layer]----------------------//

CSandbox::CSandbox()
{
	PushLayer(new ExampleLayer());
	PushOverlay(new ARC::ImGuiLayer());
}
