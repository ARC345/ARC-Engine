#include <ARC.h>
#include "ARC\Events\Event.h"
#include "SandboxApp.h"
#include "ARC\GUI\ImGuiLayer.h"
#include "ARC\Helpers\Helpers.h"
#include "ARC\Types\vector.h"
#include "imgui\imgui.h"
#include "ARC\Types\TybeBase.h"

ARC::Core::CApplication* ARC::Core::CreateApplication()
{
	return new CSandbox;
}

//-------------------[Layer]----------------------//
void CExampleLayer::OnUpdate()
{
}

void CExampleLayer::OnEvent(ARC::CEvent& _event)
{
}

void CExampleLayer::OnGuiRender()
{
	ImGui::Begin("Test");
	ImGui::Text("HelloWorld");
	ImGui::End();
	ARC_CORE_INFO(ARC::Base::CObjectBase::GetName());
}

//-------------------[Layer]----------------------//
CSandbox::CSandbox()
{
	PushLayer(new CExampleLayer());
}
