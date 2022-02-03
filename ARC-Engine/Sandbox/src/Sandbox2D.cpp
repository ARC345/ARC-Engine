//--------EntryPoint-----------------
#include "ARC/Core/EntryPoint.h"
//-----------------------------------

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
#include "ARC/Renderer/Color.h"
#include "ARC/Renderer/Texture.h"

CSandbox2D::CSandbox2D() :
	CLayer("Sandbox2D"),
	m_CameraController(1280.f / 780.f, true)
{

}

void CSandbox2D::OnAttach()
{
	m_CheckerboardTexture = ARC::CTexture2D::Create("assets/textures/Checkerboard.png");
}

void CSandbox2D::OnDetach()
{

}

void CSandbox2D::OnUpdate(float _DeltaTime)
{
	m_CameraController.OnUpdate(_DeltaTime);

	ARC::CRenderCommand::SetClearColour({ .1f, .1f, .1f, 1.f });
	ARC::CRenderCommand::Clear();

	ARC::CRenderer2D::BeginScene(m_CameraController.GetCamera());

	//ARC::CRenderer::Submit(m_TriangleShader, m_TriangleVertexArray);
	ARC::CRenderer2D::DrawQuad({0.0f, 0.0f}, -10.0f, {1.0f, 15.0f}, 1, ARC::CColor::Blue);
	ARC::CRenderer2D::DrawQuad({0.0f, 0.0f}, 0, {10.0f, 10.f}, 0, m_CheckerboardTexture);
	ARC::CRenderer2D::EndScene();

	m_CameraController.GetCamera().RecalculateViewProjectionMatrix();
}

void CSandbox2D::OnEvent(ARC::CEvent& _Event)
{
	m_CameraController.OnEvent(_Event);
}

void CSandbox2D::OnGuiRender()
{
	ImGui::Begin("Sandbox");
	if (ImGui::TreeNode("Squares"))
	{
		ImGui::ColorEdit4("SQ_Colour", SQ_Colour.Data());
		ImGui::TreePop();
	}
	ImGui::End();
}
