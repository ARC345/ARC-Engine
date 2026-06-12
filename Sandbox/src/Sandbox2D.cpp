#include "PCH/arc_pch.h"
#include "Sandbox2D.h"
#include "ARC/Renderer/Buffer.h"
#include "ARC/Renderer/Shader.h"
#include "ARC/Renderer/Texture.h"
#include "ARC/Renderer/Renderer.h"
#include "ARC/Renderer/Renderer2D.h"
#include "ARC/Renderer/VertexArray.h"
#include "ARC/Renderer/SubTexture2D.h"
#include "ARC/Renderer/RenderCommand.h"
#include "ARC/Renderer/ParticleModifier2D.h"
#include "imgui/imgui.h"
#include "glm/glm/gtc/type_ptr.inl"
#include "glm/glm/ext/matrix_transform.hpp"
#include "glm/glm/ext/matrix_float4x4.hpp"
#include "Platform/OpenGl/OpenGLShader.h"
#include "ARC/Core/Log.h"
#include "ARC/Core/Ini.h"
#include "ARC/Types/Vector.h"
#include "ARC/Types/Delegate.h"

namespace arc { static bool bStressTest = false; }
static const uint32_t map_width = 24;
static uint32_t map_height = 12;
static const char* map_tiles =
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

sandbox_2d::sandbox_2d() :
	layer("Sandbox2D"),
	m_CameraController(1280.f / 780.f, true)
{
	ParticleSystem = arc::create_ref<arc::particle_system_2d>();
	ParticleSystem->Defaults.bIsActive=false;
	ParticleSystem->Defaults.Scale = {0.4f, 0.4f};
	ParticleSystem->Defaults.Color = arc::CColor::Red;
	ParticleSystem->Defaults.Velocity = {0.f, 3.f};
	ParticleSystem->Defaults.Life = 1.1f;

	ParticleSystem->AddParticleModifier(arc::variation_application_time::OnEmit, new arc::particle_variation_modifier_2d(&arc::particle_2d::Life, 0.3f));
	//ParticleSystem->AddParticleModifier(arc::variation_application_time::OnEmit, new arc::particle_variation_modifier_2d(&arc::particle_2d::Color, arc::CColor(0.1f, 0.1f, 0.1f, 0.f)));
	ParticleSystem->AddParticleModifier(arc::variation_application_time::OnEmit, new arc::particle_variation_modifier_2d(&arc::particle_2d::Location, arc::vec2(0.5f, 0.5f)));
	ParticleSystem->AddParticleModifier(arc::variation_application_time::OnEmit, new arc::particle_variation_modifier_2d(&arc::particle_2d::Scale, arc::vec2(0.2f, 0.2f)));
	ParticleSystem->AddParticleModifier(arc::variation_application_time::OnEmit, new arc::particle_variation_modifier_2d(&arc::particle_2d::Velocity, arc::vec2(0.6f, 1.0f)));
	ParticleSystem->AddParticleModifier(arc::variation_application_time::on_update, new arc::particle_lifetime_modifier_2d(&arc::particle_2d::Color, arc::CColor(1.f,1.f,0.f,1.f)));
	ParticleSystem->AddParticleModifier(arc::variation_application_time::on_update, new arc::particle_lifetime_modifier_2d<Flip<Quadratic>, arc::vec2>(&arc::particle_2d::Scale, arc::vec2::ZeroVector));
	
	Smoke = arc::create_ref<arc::particle_system_2d>();
	Smoke->Defaults.bIsActive=false;
	Smoke->Defaults.Scale = {0.4f, 0.4f};
	Smoke->Defaults.Color = {0.2f, 0.2f, 0.2f, 1.f};

	Smoke->Defaults.Velocity = {0.f, 3.f};
	Smoke->Defaults.Life = 1.1f;
	Smoke->AddParticleModifier(arc::variation_application_time::OnEmit, new arc::particle_variation_modifier_2d(&arc::particle_2d::Life, 0.3f));
	Smoke->AddParticleModifier(arc::variation_application_time::OnEmit, new arc::particle_variation_modifier_2d(&arc::particle_2d::Color, arc::CColor(0.1f, 0.1f, 0.1f, 0.f)));
	Smoke->AddParticleModifier(arc::variation_application_time::OnEmit, new arc::particle_variation_modifier_2d(&arc::particle_2d::Location, arc::vec2(1.f, 1.f)));
	Smoke->AddParticleModifier(arc::variation_application_time::OnEmit, new arc::particle_variation_modifier_2d(&arc::particle_2d::Scale, arc::vec2(0.2f, 0.2f)));
	Smoke->AddParticleModifier(arc::variation_application_time::OnEmit, new arc::particle_variation_modifier_2d(&arc::particle_2d::Velocity, arc::vec2(0.6f, 1.0f)));
	Smoke->AddParticleModifier(arc::variation_application_time::on_update, new arc::particle_lifetime_modifier_2d<Flip<Quadratic>, arc::vec2>(&arc::particle_2d::Scale, arc::vec2::ZeroVector));
	
}

void sandbox_2d::on_attach()
{
	arc::CIniFile inif;
	arc::multicast_delegate<void(const char*)> x;
	x.bind<&sandbox_2d::___Print___>(this);
	x.bind<&sandbox_2d::___Print___>(this);
	x.bind<&sandbox_2d::___Print___>(this);
	x.bind<&sandbox_2d::___Print___>(this);
	x.bind([](const char* v) -> void { ARC_TRACE(v); });
	x("Hello");
	
	arc::delegate<void(const char*)> y;
	y.bind<&sandbox_2d::___Print___>(this);
	y.bind<&sandbox_2d::___Print___>(this);
	y.bind<&sandbox_2d::___Print___>(this);
	y.bind<&sandbox_2d::___Print___>(this);
	y("Hi");

	arc::vec3 abxd;
	ARC_TRACE("{}", abxd.to_string());

	//arc::Experimental::EntityManager em(10);
	//for (size_t i = 0; i < 100; i++)
	//{
	//	em.create_entity();
	//}
	
	m_CheckerboardTexture = arc::texture_2d::create("assets/textures/Checkerboard.png");
	m_Spritesheet = arc::texture_2d::create("assets/textures/RPGpack_sheet_2X.png");

	map_height = strlen(map_tiles) / map_width;

	m_Tree = arc::sub_texture_2d::CreateFromCoords(m_Spritesheet, {2.f, 1.f}, {128.f, 128.f}, {1, 2});
	s_textureMap['W'] = arc::sub_texture_2d::CreateFromCoords(m_Spritesheet, { 11.f, 11.f }, { 128.f, 128.f });
	s_textureMap['D'] = arc::sub_texture_2d::CreateFromCoords(m_Spritesheet, { 6.f, 11.f }, { 128.f, 128.f });
	ParticleSystem->Defaults.Texture = m_CheckerboardTexture;
	m_CameraController.set_zoom_level(1.f);

	arc::SFrameBufferSpecifications frame_buffer_specs;
	frame_buffer_specs.Width = 1280;
	frame_buffer_specs.Height = 720;
}

void sandbox_2d::on_detach()
{
	arc::instrumentor::Get().end_session();
}

void sandbox_2d::on_update(float delta_time)
{
	ARC_PROFILE_FUNCTION();
		
	{
		ARC_PROFILE_SCOPE("Stuff");
		m_CameraController.on_update(delta_time);

		arc::render_command::set_clear_colour({ .1f, .1f, .1f, 1.f });
		arc::render_command::clear();
	}
	
	arc::CRenderer2D::ResetStats();
	{
		ARC_PROFILE_SCOPE("Stuff2");

		arc::CRenderer2D::begin_scene(m_CameraController.get_camera());

		ParticleSystem->Emit();
		ParticleSystem->on_update(delta_time);
		ParticleSystem->OnRender();
		
		Smoke->Emit();
		Smoke->on_update(delta_time);
		Smoke->OnRender();
		
		for (size_t x = 0; x < map_width; x++)
		{
			for (size_t y = 0; y < map_height; y++)
			{
				char tt = map_tiles[x+y*map_width];
				arc::ref<arc::sub_texture_2d> tex;
				if (s_textureMap.find(tt) != s_textureMap.end())
					tex = s_textureMap[tt];
				else
					continue;

				arc::CRenderer2D::draw_quad({ (float)x-map_width/2, (float)map_height- y-map_height/2 }, 0.f, arc::vec2(1.f, 1.f), .5f, arc::CColor::White, tex, {1.f, 1.f});
			}
		}
		arc::CRenderer2D::draw_quad({ 0.0f, 2.f }, 0, arc::vec2(1.f, 2.f), 0.6f, arc::CColor::White, m_Tree, {1.f, 1.f});


		if (arc::bStressTest)
		{
			arc::primitive_2d Quad;
			Quad.Color = arc::FColor::Red;

			arc::CRenderer2D::draw_quad({ 0.0f, 0.0f }, 0, { 50.0f, 50.f }, 0.f, arc::CColor::White, m_CheckerboardTexture);
			for (float x = -10.f; x <= 10; x += 0.5)
			{
				for (float y = -10; y <= 10; y += 0.5)
				{
					Quad.Transform.Location = { x, y };
					Quad.Transform.Scale = { 0.45f, 0.45f };
					Quad.Transform.ZOrder = 0.1f;
					Quad.Color = arc::FColor((x + 5) / 10, 0.4f, (y + 5) / 10, 0.69f);
					arc::CRenderer2D::draw_quad(Quad);
				}
			}
			prev += 10.f * delta_time;

			arc::primitive_2d Quad2;
			Quad2.Transform.Location = { 0.f, 0.f };
			Quad2.Transform.Rotation = glm::radians(prev);
			Quad2.Transform.Scale = { 10.f, 10.f };
			Quad2.Transform.ZOrder = 0.2f;
			Quad2.Color = arc::FColor::Red;

			arc::CRenderer2D::draw_quad(Quad2);
			Quad2.Transform.Rotation = glm::radians(prev + 30);
			arc::CRenderer2D::draw_quad(Quad2);
			Quad2.Transform.Rotation = glm::radians(prev + 60);
			arc::CRenderer2D::draw_quad(Quad2);
			Quad2.Transform.Rotation = glm::radians(prev + 90);

			Quad2.Transform.Scale = { 9.f, 9.f };
			Quad2.Transform.ZOrder += 0.001f;
			Quad2.Color = arc::CColor::Yellow;

			Quad2.Transform.Rotation = glm::radians(-prev - 30);
			arc::CRenderer2D::draw_quad(Quad2);
			Quad2.Transform.Rotation = glm::radians(-prev - 60);
			arc::CRenderer2D::draw_quad(Quad2);
			Quad2.Transform.Rotation = glm::radians(-prev - 90);
			arc::CRenderer2D::draw_quad(Quad2);

			Quad2.Transform.Scale = { 8.f, 8.f };
			Quad2.Transform.ZOrder += 0.001f;
			Quad2.Color = arc::CColor::Black;

			Quad2.Transform.Rotation = glm::radians(prev + 30);
			arc::CRenderer2D::draw_quad(Quad2);
			Quad2.Transform.Rotation = glm::radians(prev + 60);
			arc::CRenderer2D::draw_quad(Quad2);
			Quad2.Transform.Rotation = glm::radians(prev + 90);
			arc::CRenderer2D::draw_quad(Quad2);

			Quad2.Transform.Scale = { 7.f, 7.f };
			Quad2.Transform.ZOrder += 0.001f;
			Quad2.Color = arc::CColor::Yellow;

			Quad2.Transform.Rotation = glm::radians(-prev - 30);
			arc::CRenderer2D::draw_quad(Quad2);
			Quad2.Transform.Rotation = glm::radians(-prev - 60);
			arc::CRenderer2D::draw_quad(Quad2);
			Quad2.Transform.Rotation = glm::radians(-prev - 90);
			arc::CRenderer2D::draw_quad(Quad2);

			Quad2.Transform.Scale = { 6.f, 6.f };
			Quad2.Transform.ZOrder += 0.001f;
			Quad2.Color = arc::CColor::Black;

			Quad2.Transform.Rotation = glm::radians(prev + 30);
			arc::CRenderer2D::draw_quad(Quad2);
			Quad2.Transform.Rotation = glm::radians(prev + 60);
			arc::CRenderer2D::draw_quad(Quad2);
			Quad2.Transform.Rotation = glm::radians(prev + 90);
			arc::CRenderer2D::draw_quad(Quad2);

			Quad2.Transform.Scale = { 5.f, 5.f };
			Quad2.Transform.ZOrder += 0.001f;
			Quad2.Color = arc::CColor::Yellow;

			Quad2.Transform.Rotation = glm::radians(-prev - 30);
			arc::CRenderer2D::draw_quad(Quad2);
			Quad2.Transform.Rotation = glm::radians(-prev - 60);
			arc::CRenderer2D::draw_quad(Quad2);
			Quad2.Transform.Rotation = glm::radians(-prev - 90);
			arc::CRenderer2D::draw_quad(Quad2);

			Quad2.Transform.Scale = { 4.f, 4.f };
			Quad2.Transform.ZOrder += 0.001f;
			Quad2.Color = arc::CColor::Black;

			Quad2.Transform.Rotation = glm::radians(prev + 30);
			arc::CRenderer2D::draw_quad(Quad2);
			Quad2.Transform.Rotation = glm::radians(prev + 60);
			arc::CRenderer2D::draw_quad(Quad2);
			Quad2.Transform.Rotation = glm::radians(prev + 90);
			arc::CRenderer2D::draw_quad(Quad2);

			Quad2.Transform.Scale = { 3.f, 3.f };
			Quad2.Transform.ZOrder += 0.001f;
			Quad2.Color = arc::CColor::Yellow;

			Quad2.Transform.Rotation = glm::radians(-prev - 30);
			arc::CRenderer2D::draw_quad(Quad2);
			Quad2.Transform.Rotation = glm::radians(-prev - 60);
			arc::CRenderer2D::draw_quad(Quad2);
			Quad2.Transform.Rotation = glm::radians(-prev - 90);
			arc::CRenderer2D::draw_quad(Quad2);

			Quad2.Transform.Scale = { 2.f, 2.f };
			Quad2.Transform.ZOrder += 0.001f;
			Quad2.Color = arc::CColor::Black;

			Quad2.Transform.Rotation = glm::radians(prev + 30);
			arc::CRenderer2D::draw_quad(Quad2);
			Quad2.Transform.Rotation = glm::radians(prev + 60);
			arc::CRenderer2D::draw_quad(Quad2);
			Quad2.Transform.Rotation = glm::radians(prev + 90);
			arc::CRenderer2D::draw_quad(Quad2);

			Quad2.Transform.Scale = { 1.f, 1.f };
			Quad2.Transform.ZOrder += 0.001f;
			Quad2.Color = arc::FColor::Red;

			Quad2.Transform.Rotation = glm::radians(-prev - 30);
			arc::CRenderer2D::draw_quad(Quad2);
			Quad2.Transform.Rotation = glm::radians(-prev - 60);
			arc::CRenderer2D::draw_quad(Quad2);
			Quad2.Transform.Rotation = glm::radians(-prev - 90);
			arc::CRenderer2D::draw_quad(Quad2);

			arc::CRenderer2D::draw_quad(arc::vec2::ZeroVector, 0.f, { 50.0f, 50.f }, 0.f, arc::CColor::White, m_CheckerboardTexture);

		}
		arc::CRenderer2D::end_scene();
		m_CameraController.get_camera().RecalculateViewProjectionMatrix();
	}
}

void sandbox_2d::on_event(arc::event& event)
{
	ARC_PROFILE_FUNCTION();
	m_CameraController.on_event(event);
}

void sandbox_2d::on_gui_render()
{
	ARC_PROFILE_FUNCTION();
	
	auto stats = arc::CRenderer2D::get_stats();
	ImGui::begin("Renderer2D");
	ImGui::Image((void*)m_CheckerboardTexture->get_renderer_id(), ImVec2(100.f, 100.f));
	ImGui::Text("DrawCalls: %d", stats.DrawCalls);
	ImGui::Text("QuadCount: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.get_total_vertex_count());
	ImGui::Text("Indices: %d", stats.get_total_index_count());
	ImGui::end();
	ImGui::begin((arc::bStressTest ? "StressTest" : "Sandbox"));
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
	if (ImGui::Button(arc::bStressTest ? "Go to Sandbox?" : "Go to StressTest?")) { arc::bStressTest = !arc::bStressTest; }
	ImGui::end();
}

void sandbox_2d::___Print___(const char* _)
{
	ARC_WARN(_);
}
