#include "PCH\arc_pch.h"
#include "Sandbox2D.h"
#include "ARC\Renderer\Buffer.h"
#include "ARC\Renderer\Shader.h"
#include "ARC\Renderer\VertexArray.h"
#include "imgui\imgui.h"
#include "glm\glm\gtc\type_ptr.inl"
#include "glm\glm\ext\matrix_transform.hpp"
#include "glm\glm\ext\matrix_float4x4.hpp"
#include "Platform\OpenGl\OpenGLShader.h"
#include "ARC\Renderer\Renderer.h"
#include "ARC\Renderer\RenderCommand.h"
#include "ARC/Renderer/Renderer2D.h"
#include "ARC/Renderer/Texture.h"
#include "ARC/Types/Vector.h"
#include "ARC\Core\Log.h"

CSandbox2D::CSandbox2D() :
	CLayer("Sandbox2D"),
	m_CameraController(1280.f / 780.f, true)
{

}

void CSandbox2D::OnAttach()
{
	CIniFile inif();
	m_CheckerboardTexture = ARC::CTexture2D::Create("assets/textures/Checkerboard.png");
}

void CSandbox2D::OnDetach()
{


	ARC::CInstrumentor::Get().EndSession();
}

void CSandbox2D::OnUpdate(float _DeltaTime)
{
	ARC_PROFILE_FUNCTION();
	{
		ARC_PROFILE_SCOPE("Stuff");
		m_CameraController.OnUpdate(_DeltaTime);

		ARC::CRenderCommand::SetClearColour({ .1f, .1f, .1f, 1.f });
		ARC::CRenderCommand::Clear();
	}
	
	{
		ARC_PROFILE_SCOPE("Stuff2");

		ARC::CRenderer2D::BeginScene(m_CameraController.GetCamera());

		//ARC::CRenderer::Submit(m_TriangleShader, m_TriangleVertexArray);
		//prev += 10* _DeltaTime;

		//ARC::CPrimitive2D Quad;
		//Quad.Transform.Location = {0.f, 0.f};
		//Quad.Transform.Rotation = 0; //glm::radians(prev);
		//Quad.Transform.Scale = { 10.f, 10.f };
		//Quad.Transform.ZOrder = 0;
		//Quad.Color = ARC::CColor::Red;
		//
		//ARC::CRenderer2D::DrawQuad(Quad);
		ARC::CRenderer2D::DrawQuad({0.f, 1.f}, 0.f, {1.f, 1.f}, 0, ARC::CColor::Red);
		ARC::CRenderer2D::DrawQuad({0.f, -1.f}, 0.f, {1.f, 1.f}, 0, ARC::CColor::Blue);
		//
		//ARC::CRenderer2D::DrawQuad(Quad);
		//Quad.Transform.Rotation = glm::radians(prev+30);
		//ARC::CRenderer2D::DrawQuad(Quad);
		//Quad.Transform.Rotation = glm::radians(prev+60);
		//ARC::CRenderer2D::DrawQuad(Quad);
		//Quad.Transform.Rotation = glm::radians(prev+90);
		//ARC::CRenderer2D::DrawQuad(Quad);
		//Quad.Transform.Rotation = glm::radians(prev+120);
		//ARC::CRenderer2D::DrawQuad(Quad);
		//Quad.Transform.Rotation = glm::radians(prev+150);
		//ARC::CRenderer2D::DrawQuad(Quad);
		//Quad.Transform.Rotation = glm::radians(prev+180);
		//ARC::CRenderer2D::DrawQuad(Quad);
		//Quad.Transform.Rotation = glm::radians(prev+210);
		//ARC::CRenderer2D::DrawQuad(Quad);
		//Quad.Transform.Rotation = glm::radians(prev+240);
		//ARC::CRenderer2D::DrawQuad(Quad);
		//Quad.Transform.Rotation = glm::radians(prev+270);
		//ARC::CRenderer2D::DrawQuad(Quad);
		//Quad.Transform.Rotation = glm::radians(prev+300);
		//ARC::CRenderer2D::DrawQuad({ 0.0f, 0.0f }, 0, { 1.0f, 1.f }, 0, { 1, 1 }, ARC::CColor::White, m_CheckerboardTexture);
		ARC::CRenderer2D::EndScene();

		m_CameraController.GetCamera().RecalculateViewProjectionMatrix();
	}
}

void CSandbox2D::OnEvent(ARC::CEvent& _Event)
{
	ARC_PROFILE_FUNCTION();
	m_CameraController.OnEvent(_Event);
}

void CSandbox2D::OnGuiRender()
{
	ARC_PROFILE_FUNCTION();
	ImGui::Begin("Sandbox");
	if (ImGui::TreeNode("Squares"))
	{
		ImGui::ColorEdit4("SQ_Colour", SQ_Colour.Data());
		ImGui::TreePop();
	}
	ImGui::End();
}
