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
#include "ARC\Core\Ini.h"

namespace ARC{ static bool bStressTest = false; }

CSandbox2D::CSandbox2D() :
	CLayer("Sandbox2D"),
	m_CameraController(1280.f / 780.f, true)
{

}

void CSandbox2D::OnAttach()
{
	ARC::CIniFile inif;
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
	
	ARC::CRenderer2D::ResetStats();
	{
		ARC_PROFILE_SCOPE("Stuff2");

		ARC::CRenderer2D::BeginScene(m_CameraController.GetCamera());

		if (ARC::bStressTest)
		{
			ARC::CPrimitive2D Quad;
			Quad.Color = ARC::CColor::Red;
			
			ARC::CRenderer2D::DrawQuad({ 0.0f, 0.0f }, 0, { 50.0f, 50.f }, 0, { 1, 1 }, ARC::CColor::White, m_CheckerboardTexture);
			for (float x = -10.f; x <= 10; x+=0.5)
			{
				for (float y = -10; y <= 10; y += 0.5)
				{
					Quad.Transform.Location = {x, y};
					Quad.Transform.Scale = {0.45, 0.45 };
					Quad.Transform.ZOrder = 0.1;
					Quad.Color = ARC::CColor((x+5)/10, 0.4, (y+5)/10, 0.69);
					ARC::CRenderer2D::DrawQuad(Quad);
				}
			}

		}
		{
			prev += 10* _DeltaTime;
	
			ARC::CPrimitive2D Quad;
			Quad.Transform.Location = { 0.f, 0.f };
			Quad.Transform.Rotation = glm::radians(prev);
			Quad.Transform.Scale = { 10.f, 10.f };
			Quad.Transform.ZOrder = 0.2;
			Quad.Color = ARC::CColor::Red;
	
			ARC::CRenderer2D::DrawQuad(Quad);
			Quad.Transform.Rotation = glm::radians(prev+30);
			ARC::CRenderer2D::DrawQuad(Quad);
			Quad.Transform.Rotation = glm::radians(prev+60);
			ARC::CRenderer2D::DrawQuad(Quad);
			Quad.Transform.Rotation = glm::radians(prev+90);
	
			Quad.Transform.Scale = { 9.f, 9.f };
			Quad.Transform.ZOrder += 0.001;
			Quad.Color = ARC::CColor::Yellow;
	
			Quad.Transform.Rotation = glm::radians(-prev - 30);
			ARC::CRenderer2D::DrawQuad(Quad);
			Quad.Transform.Rotation = glm::radians(-prev - 60);
			ARC::CRenderer2D::DrawQuad(Quad);
			Quad.Transform.Rotation = glm::radians(-prev - 90);
			ARC::CRenderer2D::DrawQuad(Quad);
	
			Quad.Transform.Scale = { 8.f, 8.f };
			Quad.Transform.ZOrder += 0.001;
			Quad.Color = ARC::CColor::Black;
	
			Quad.Transform.Rotation = glm::radians(prev + 30);
			ARC::CRenderer2D::DrawQuad(Quad);
			Quad.Transform.Rotation = glm::radians(prev + 60);
			ARC::CRenderer2D::DrawQuad(Quad);
			Quad.Transform.Rotation = glm::radians(prev + 90);
			ARC::CRenderer2D::DrawQuad(Quad);
			
			Quad.Transform.Scale = { 7.f, 7.f };
			Quad.Transform.ZOrder += 0.001;
			Quad.Color = ARC::CColor::Yellow;
	
			Quad.Transform.Rotation = glm::radians(-prev - 30);
			ARC::CRenderer2D::DrawQuad(Quad);
			Quad.Transform.Rotation = glm::radians(-prev - 60);
			ARC::CRenderer2D::DrawQuad(Quad);
			Quad.Transform.Rotation = glm::radians(-prev - 90);
			ARC::CRenderer2D::DrawQuad(Quad);
	
			Quad.Transform.Scale = { 6.f, 6.f };
			Quad.Transform.ZOrder += 0.001;
			Quad.Color = ARC::CColor::Black;
	
			Quad.Transform.Rotation = glm::radians(prev + 30);
			ARC::CRenderer2D::DrawQuad(Quad);
			Quad.Transform.Rotation = glm::radians(prev + 60);
			ARC::CRenderer2D::DrawQuad(Quad);
			Quad.Transform.Rotation = glm::radians(prev + 90);
			ARC::CRenderer2D::DrawQuad(Quad);
	
			Quad.Transform.Scale = {5.f, 5.f};
			Quad.Transform.ZOrder += 0.001;
			Quad.Color = ARC::CColor::Yellow;
			
			Quad.Transform.Rotation = glm::radians(-prev-30);
			ARC::CRenderer2D::DrawQuad(Quad);
			Quad.Transform.Rotation = glm::radians(-prev-60);
			ARC::CRenderer2D::DrawQuad(Quad);
			Quad.Transform.Rotation = glm::radians(-prev-90);
			ARC::CRenderer2D::DrawQuad(Quad);
			
			Quad.Transform.Scale = {4.f, 4.f};
			Quad.Transform.ZOrder += 0.001;
			Quad.Color = ARC::CColor::Black;
			
			Quad.Transform.Rotation = glm::radians(prev+30);
			ARC::CRenderer2D::DrawQuad(Quad);
			Quad.Transform.Rotation = glm::radians(prev+60);
			ARC::CRenderer2D::DrawQuad(Quad);
			Quad.Transform.Rotation = glm::radians(prev+90);
			ARC::CRenderer2D::DrawQuad(Quad);
	
			Quad.Transform.Scale = { 3.f, 3.f };
			Quad.Transform.ZOrder += 0.001;
			Quad.Color = ARC::CColor::Yellow;
	
			Quad.Transform.Rotation = glm::radians(-prev - 30);
			ARC::CRenderer2D::DrawQuad(Quad);
			Quad.Transform.Rotation = glm::radians(-prev - 60);
			ARC::CRenderer2D::DrawQuad(Quad);
			Quad.Transform.Rotation = glm::radians(-prev - 90);
			ARC::CRenderer2D::DrawQuad(Quad);
	
			Quad.Transform.Scale = { 2.f, 2.f };
			Quad.Transform.ZOrder += 0.001;
			Quad.Color = ARC::CColor::Black;
	
			Quad.Transform.Rotation = glm::radians(prev + 30);
			ARC::CRenderer2D::DrawQuad(Quad);
			Quad.Transform.Rotation = glm::radians(prev + 60);
			ARC::CRenderer2D::DrawQuad(Quad);
			Quad.Transform.Rotation = glm::radians(prev + 90);
			ARC::CRenderer2D::DrawQuad(Quad);
	
			Quad.Transform.Scale = { 1.f, 1.f };
			Quad.Transform.ZOrder += 0.001;
			Quad.Color = ARC::CColor::Red;
	
			Quad.Transform.Rotation = glm::radians(-prev - 30);
			ARC::CRenderer2D::DrawQuad(Quad);
			Quad.Transform.Rotation = glm::radians(-prev - 60);
			ARC::CRenderer2D::DrawQuad(Quad);
			Quad.Transform.Rotation = glm::radians(-prev - 90);
			ARC::CRenderer2D::DrawQuad(Quad);
	
			ARC::CRenderer2D::DrawQuad({ 0.0f, 0.0f }, 0, { 50.0f, 50.f }, 0, { 1, 1 }, ARC::CColor::White, m_CheckerboardTexture);
			
		}
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
	auto stats = ARC::CRenderer2D::GetStats();
	ImGui::Begin("Renderer2D");
	ImGui::Text("DrawCalls: %d", stats.DrawCalls);
	ImGui::Text("QuadCount: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::End();
	ImGui::Begin((ARC::bStressTest ? "StressTest" : "Sandbox"));
	if (ImGui::TreeNode("Squares"))
	{
		ImGui::ColorEdit4("SQ_Colour", SQ_Colour.Data());
		ImGui::TreePop();
	}
	if(ImGui::Button(ARC::bStressTest ? "Go to Sandbox?" : "Go to StressTest?")) { ARC::bStressTest = !ARC::bStressTest; }
	ImGui::End();
}
