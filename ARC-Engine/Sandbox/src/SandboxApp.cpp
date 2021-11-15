#include <ARC.h>
#include "ARC\Events\Event.h"
#include "SandboxApp.h"
#include "ARC\GUI\ImGuiLayer.h"

ARC::Core::CApplication* ARC::Core::CreateApplication()
{
	return new CSandbox;
}


//-------------------[Layer]----------------------//
void ExampleLayer::OnUpdate()
{
	ARC_INFO("ExampleLayer::Update");
}

void ExampleLayer::OnEvent(ARC::CEvent& _event)
{
	ARC_TRACE("{0}", _event.ToString());
}
//-------------------[Layer]----------------------//

CSandbox::CSandbox()
{
	PushLayer(new ExampleLayer());
	PushOverlay(new ARC::ImGuiLayer());
}
