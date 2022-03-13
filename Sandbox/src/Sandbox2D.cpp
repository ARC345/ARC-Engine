#include "PCH\arc_pch.h"
#include "Sandbox2D.h"
#include "ARC\Renderer\Buffer.h"
#include "ARC\Renderer\Shader.h"
#include "ARC\Renderer\VertexArray.h"
#include "imgui\imgui.h"
#include "glm\glm\gtc\type_ptr.inl"
#include "glm\glm\ext\matrix_transform.hpp"
#include "ARC\Objects\ParticleModifier2D.h"
#include "glm\glm\ext\matrix_float4x4.hpp"
#include "Platform\OpenGl\OpenGLShader.h"
#include "ARC\Renderer\Renderer.h"
#include "ARC\Renderer\RenderCommand.h"
#include "ARC/Renderer/Renderer2D.h"
#include "ARC/Renderer/Texture.h"
#include "ARC/Types/Vector.h"
#include "ARC\Core\Log.h"
#include "ARC\Core\Ini.h"
#include "ARC\Renderer\SubTexture2D.h"

namespace ARC{ static bool bStressTest = false; }
static const uint32_t s_MapWidth = 24;
static uint32_t s_MapHeight = 12;
static const char* s_MapTiles = 
	"WWWWWWWWWWWWWWWWWWWWWWWW"
	"WWWWDDDDDDDDWWWWWWWWWWWW"
	"WWWDDDDDDDDDDDDDDDWWWWWW"
	"WWWDDDDDDDDDDDDDDDDDWWWW"
	"WWWDDDDDDDDDDDDDDDDDDWWW"
	"WWWWWWDDDDDDDDDDDDDDWWWW"
	"WWWWDDDDDDDDWWDDDDDDWWWW"
	"WWWDDDDDDDDWWWWDDDDDWWWW"
	"WWWWDDDDDDDDWWDDDDDDWWWW"
	"WWWWWWDDDDDDDDDWWWWWWWWW"
	"WWWWWWWWDDDDDDDWWWWWWWWW"
	"WWWWWWWWWWWWWWWWWWWWWWWW";

CSandbox2D::CSandbox2D() :
	CLayer("Sandbox2D"),
	m_CameraController(1280.f / 780.f, true)
{
	ParticleSystem = ARC::CreateRef<ARC::CParticleSystem2D>();
	ParticleSystem->Defaults.bIsActive=false;
	ParticleSystem->Defaults.Scale = {0.4f, 0.4f};
	ParticleSystem->Defaults.Color = ARC::CColor::Red;
	ParticleSystem->Defaults.Velocity = {0.f, 3.f};
	ParticleSystem->Defaults.Life = 1.1f;

	ParticleSystem->AddParticleModifier(ARC::EVariationApplicationTime::OnEmit, new ARC::CParticleVariationModifier2D(&ARC::CParticle2D::Life, 0.3f));
	//ParticleSystem->AddParticleModifier(ARC::EVariationApplicationTime::OnEmit, new ARC::CParticleVariationModifier2D(&ARC::CParticle2D::Color, ARC::CColor(0.1f, 0.1f, 0.1f, 0.f)));
	ParticleSystem->AddParticleModifier(ARC::EVariationApplicationTime::OnEmit, new ARC::CParticleVariationModifier2D(&ARC::CParticle2D::Location, ARC::FVec2(0.5f, 0.5f)));
	ParticleSystem->AddParticleModifier(ARC::EVariationApplicationTime::OnEmit, new ARC::CParticleVariationModifier2D(&ARC::CParticle2D::Scale, ARC::FVec2(0.2f, 0.2f)));
	ParticleSystem->AddParticleModifier(ARC::EVariationApplicationTime::OnEmit, new ARC::CParticleVariationModifier2D(&ARC::CParticle2D::Velocity, ARC::FVec2(0.6f, 1.0f)));
	ParticleSystem->AddParticleModifier(ARC::EVariationApplicationTime::OnUpdate, new ARC::CParticleLifetimeModifier2D(&ARC::CParticle2D::Color, ARC::CColor(1.f,1.f,0.f,1.f)));
	ParticleSystem->AddParticleModifier(ARC::EVariationApplicationTime::OnUpdate, new ARC::CParticleLifetimeModifier2D<Flip<Quadratic>, ARC::FVec2>(&ARC::CParticle2D::Scale, ARC::FVec2::ZeroVector));
	
	Smoke = ARC::CreateRef<ARC::CParticleSystem2D>();
	Smoke->Defaults.bIsActive=false;
	Smoke->Defaults.Scale = {0.4f, 0.4f};
	Smoke->Defaults.Color = {0.2f, 0.2f, 0.2f, 1.f};

	Smoke->Defaults.Velocity = {0.f, 3.f};
	Smoke->Defaults.Life = 1.1f;
	Smoke->AddParticleModifier(ARC::EVariationApplicationTime::OnEmit, new ARC::CParticleVariationModifier2D(&ARC::CParticle2D::Life, 0.3f));
	Smoke->AddParticleModifier(ARC::EVariationApplicationTime::OnEmit, new ARC::CParticleVariationModifier2D(&ARC::CParticle2D::Color, ARC::CColor(0.1f, 0.1f, 0.1f, 0.f)));
	Smoke->AddParticleModifier(ARC::EVariationApplicationTime::OnEmit, new ARC::CParticleVariationModifier2D(&ARC::CParticle2D::Location, ARC::FVec2(1.f, 1.f)));
	Smoke->AddParticleModifier(ARC::EVariationApplicationTime::OnEmit, new ARC::CParticleVariationModifier2D(&ARC::CParticle2D::Scale, ARC::FVec2(0.2f, 0.2f)));
	Smoke->AddParticleModifier(ARC::EVariationApplicationTime::OnEmit, new ARC::CParticleVariationModifier2D(&ARC::CParticle2D::Velocity, ARC::FVec2(0.6f, 1.0f)));
	Smoke->AddParticleModifier(ARC::EVariationApplicationTime::OnUpdate, new ARC::CParticleLifetimeModifier2D<Flip<Quadratic>, ARC::FVec2>(&ARC::CParticle2D::Scale, ARC::FVec2::ZeroVector));
	
}

void CSandbox2D::OnAttach()
{
	ARC::CIniFile inif;
	m_CheckerboardTexture = ARC::CTexture2D::Create("assets/textures/Checkerboard.png");
	m_Spritesheet = ARC::CTexture2D::Create("assets/textures/RPGpack_sheet_2X.png");

	s_MapHeight = strlen(s_MapTiles) / s_MapWidth;

	m_Tree = ARC::CSubTexture2D::CreateFromCoords(m_Spritesheet, {2.f, 1.f}, {128.f, 128.f}, {1, 2});
	s_textureMap['W'] = ARC::CSubTexture2D::CreateFromCoords(m_Spritesheet, { 11.f, 11.f }, { 128.f, 128.f });
	s_textureMap['D'] = ARC::CSubTexture2D::CreateFromCoords(m_Spritesheet, { 6.f, 11.f }, { 128.f, 128.f });
	ParticleSystem->Defaults.Texture = m_CheckerboardTexture;
	m_CameraController.SetZoomLevel(1.f);

	ARC::SFrameBufferSpecifications frame_buffer_specs;
	frame_buffer_specs.Width = 1280;
	frame_buffer_specs.Height = 720;
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

		ParticleSystem->Emit();
		ParticleSystem->OnUpdate(_DeltaTime);
		ParticleSystem->OnRender();
		
		Smoke->Emit();
		Smoke->OnUpdate(_DeltaTime);
		Smoke->OnRender();

		
		for (size_t x = 0; x < s_MapWidth; x++)
		{
			for (size_t y = 0; y < s_MapHeight; y++)
			{
				char tt = s_MapTiles[x+y*s_MapWidth];
				ARC::TRef<ARC::CSubTexture2D> tex;
				if (s_textureMap.find(tt) != s_textureMap.end())
					tex = s_textureMap[tt];
				else
					continue;

				ARC::CRenderer2D::DrawQuad({ (float)x-s_MapWidth/2, (float)s_MapHeight- y-s_MapHeight/2 }, 0.f, ARC::FVec2(1.f, 1.f), .5f, ARC::CColor::White, tex, {1.f, 1.f});
			}
		}
		ARC::CRenderer2D::DrawQuad({ 0.0f, 2.f }, 0, ARC::FVec2(1.f, 2.f), 0.6f, ARC::CColor::White, m_Tree, {1.f, 1.f});


		if (ARC::bStressTest)
		{
			ARC::CPrimitive2D Quad;
			Quad.Color = ARC::CColor::Red;

			ARC::CRenderer2D::DrawQuad({ 0.0f, 0.0f }, 0, { 50.0f, 50.f }, 0.f, ARC::CColor::White, m_CheckerboardTexture);
			for (float x = -10.f; x <= 10; x += 0.5)
			{
				for (float y = -10; y <= 10; y += 0.5)
				{
					Quad.Transform.Location = { x, y };
					Quad.Transform.Scale = { 0.45f, 0.45f };
					Quad.Transform.ZOrder = 0.1f;
					Quad.Color = ARC::CColor((x + 5) / 10, 0.4f, (y + 5) / 10, 0.69f);
					ARC::CRenderer2D::DrawQuad(Quad);
				}
			}
			prev += 10.f * _DeltaTime;

			ARC::CPrimitive2D Quad2;
			Quad2.Transform.Location = { 0.f, 0.f };
			Quad2.Transform.Rotation = glm::radians(prev);
			Quad2.Transform.Scale = { 10.f, 10.f };
			Quad2.Transform.ZOrder = 0.2f;
			Quad2.Color = ARC::CColor::Red;

			ARC::CRenderer2D::DrawQuad(Quad2);
			Quad2.Transform.Rotation = glm::radians(prev + 30);
			ARC::CRenderer2D::DrawQuad(Quad2);
			Quad2.Transform.Rotation = glm::radians(prev + 60);
			ARC::CRenderer2D::DrawQuad(Quad2);
			Quad2.Transform.Rotation = glm::radians(prev + 90);

			Quad2.Transform.Scale = { 9.f, 9.f };
			Quad2.Transform.ZOrder += 0.001f;
			Quad2.Color = ARC::CColor::Yellow;

			Quad2.Transform.Rotation = glm::radians(-prev - 30);
			ARC::CRenderer2D::DrawQuad(Quad2);
			Quad2.Transform.Rotation = glm::radians(-prev - 60);
			ARC::CRenderer2D::DrawQuad(Quad2);
			Quad2.Transform.Rotation = glm::radians(-prev - 90);
			ARC::CRenderer2D::DrawQuad(Quad2);

			Quad2.Transform.Scale = { 8.f, 8.f };
			Quad2.Transform.ZOrder += 0.001f;
			Quad2.Color = ARC::CColor::Black;

			Quad2.Transform.Rotation = glm::radians(prev + 30);
			ARC::CRenderer2D::DrawQuad(Quad2);
			Quad2.Transform.Rotation = glm::radians(prev + 60);
			ARC::CRenderer2D::DrawQuad(Quad2);
			Quad2.Transform.Rotation = glm::radians(prev + 90);
			ARC::CRenderer2D::DrawQuad(Quad2);

			Quad2.Transform.Scale = { 7.f, 7.f };
			Quad2.Transform.ZOrder += 0.001f;
			Quad2.Color = ARC::CColor::Yellow;

			Quad2.Transform.Rotation = glm::radians(-prev - 30);
			ARC::CRenderer2D::DrawQuad(Quad2);
			Quad2.Transform.Rotation = glm::radians(-prev - 60);
			ARC::CRenderer2D::DrawQuad(Quad2);
			Quad2.Transform.Rotation = glm::radians(-prev - 90);
			ARC::CRenderer2D::DrawQuad(Quad2);

			Quad2.Transform.Scale = { 6.f, 6.f };
			Quad2.Transform.ZOrder += 0.001f;
			Quad2.Color = ARC::CColor::Black;

			Quad2.Transform.Rotation = glm::radians(prev + 30);
			ARC::CRenderer2D::DrawQuad(Quad2);
			Quad2.Transform.Rotation = glm::radians(prev + 60);
			ARC::CRenderer2D::DrawQuad(Quad2);
			Quad2.Transform.Rotation = glm::radians(prev + 90);
			ARC::CRenderer2D::DrawQuad(Quad2);

			Quad2.Transform.Scale = { 5.f, 5.f };
			Quad2.Transform.ZOrder += 0.001f;
			Quad2.Color = ARC::CColor::Yellow;

			Quad2.Transform.Rotation = glm::radians(-prev - 30);
			ARC::CRenderer2D::DrawQuad(Quad2);
			Quad2.Transform.Rotation = glm::radians(-prev - 60);
			ARC::CRenderer2D::DrawQuad(Quad2);
			Quad2.Transform.Rotation = glm::radians(-prev - 90);
			ARC::CRenderer2D::DrawQuad(Quad2);

			Quad2.Transform.Scale = { 4.f, 4.f };
			Quad2.Transform.ZOrder += 0.001f;
			Quad2.Color = ARC::CColor::Black;

			Quad2.Transform.Rotation = glm::radians(prev + 30);
			ARC::CRenderer2D::DrawQuad(Quad2);
			Quad2.Transform.Rotation = glm::radians(prev + 60);
			ARC::CRenderer2D::DrawQuad(Quad2);
			Quad2.Transform.Rotation = glm::radians(prev + 90);
			ARC::CRenderer2D::DrawQuad(Quad2);

			Quad2.Transform.Scale = { 3.f, 3.f };
			Quad2.Transform.ZOrder += 0.001f;
			Quad2.Color = ARC::CColor::Yellow;

			Quad2.Transform.Rotation = glm::radians(-prev - 30);
			ARC::CRenderer2D::DrawQuad(Quad2);
			Quad2.Transform.Rotation = glm::radians(-prev - 60);
			ARC::CRenderer2D::DrawQuad(Quad2);
			Quad2.Transform.Rotation = glm::radians(-prev - 90);
			ARC::CRenderer2D::DrawQuad(Quad2);

			Quad2.Transform.Scale = { 2.f, 2.f };
			Quad2.Transform.ZOrder += 0.001f;
			Quad2.Color = ARC::CColor::Black;

			Quad2.Transform.Rotation = glm::radians(prev + 30);
			ARC::CRenderer2D::DrawQuad(Quad2);
			Quad2.Transform.Rotation = glm::radians(prev + 60);
			ARC::CRenderer2D::DrawQuad(Quad2);
			Quad2.Transform.Rotation = glm::radians(prev + 90);
			ARC::CRenderer2D::DrawQuad(Quad2);

			Quad2.Transform.Scale = { 1.f, 1.f };
			Quad2.Transform.ZOrder += 0.001f;
			Quad2.Color = ARC::CColor::Red;

			Quad2.Transform.Rotation = glm::radians(-prev - 30);
			ARC::CRenderer2D::DrawQuad(Quad2);
			Quad2.Transform.Rotation = glm::radians(-prev - 60);
			ARC::CRenderer2D::DrawQuad(Quad2);
			Quad2.Transform.Rotation = glm::radians(-prev - 90);
			ARC::CRenderer2D::DrawQuad(Quad2);

			ARC::CRenderer2D::DrawQuad(ARC::FVec2::ZeroVector, 0.f, { 50.0f, 50.f }, 0.f, ARC::CColor::White, m_CheckerboardTexture);

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
	ImGui::Image((void*)m_CheckerboardTexture->GetRendererID(), ImVec2(100.f, 100.f));
	ImGui::Text("DrawCalls: %d", stats.DrawCalls);
	ImGui::Text("QuadCount: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::End();
	ImGui::Begin((ARC::bStressTest ? "StressTest" : "Sandbox"));
	if (ImGui::TreeNode("Particles"))
	{
		ImGui::DragFloat3("Particle Loc", ParticleSystem->Defaults.Location.Data());
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Squares"))
	{
		ImGui::ColorEdit4("SQ_Colour", SQ_Colour.Data());
		ImGui::TreePop();
	}
	if (ImGui::Button(ARC::bStressTest ? "Go to Sandbox?" : "Go to StressTest?")) { ARC::bStressTest = !ARC::bStressTest; }
	ImGui::End();
}