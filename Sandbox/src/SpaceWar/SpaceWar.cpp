#include "arc_pch.h"
#include "SpaceWar.h"
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
#include "ARC/Objects/Primitive2D.h"
#include "ARC/Types/vector.h"
#include "Player.h"
#include "Rock.h"
#include "ARC\Helpers\Math.h"
#include "ARC\Helpers\Helpers.h"
#include "ARC\Types\Transform2D.h"
#include "ARC\Core\Window.h"
#include "ARC\Events\MouseEvent.h"
#include "ARC\Events\Event.h"

static float i = 0.5;

space_war::space_war() :
	layer("SpaceWar"),
	m_CameraController(1280.f / 780.f, true)
{

}

void space_war::on_attach()
{
	arc::HPR::Random::init();
	GameState = game_state::MainMenu;
	m_Player = arc::create_ref<arc::player>();
	m_Player->Load();
	m_Player->OwningLayer = this;
	ImGuiIO io = ImGui::get_io();
	m_Font = io.Fonts->AddFontFromFileTTF("assets/fonts/Inconsolata-Medium.ttf", 120.0f);
}

void space_war::on_detach()
{
	arc::instrumentor::Get().end_session();
}

bool space_war::detect_collision()
{
	ARC_PROFILE_FUNCTION();
	arc::vec2 TLPP = arc::HPR::QUAD::get_corner<TopLeft>(m_Player->PlayerSprite.get_transform());
	arc::vec2 TRPP = arc::HPR::QUAD::get_corner<TopRight>(m_Player->PlayerSprite.get_transform());
	arc::vec2 BLPP = arc::HPR::QUAD::get_corner<BottomLeft>(m_Player->PlayerSprite.get_transform());
	arc::vec2 BRPP = arc::HPR::QUAD::get_corner<BottomRight>(m_Player->PlayerSprite.get_transform());

	std::vector<std::shared_ptr<arc::vec2>> cachedLocs;
	cachedLocs.reserve(m_Player->CollisionData.size());

	for (auto& PlayerCC : m_Player->CollisionData)
	{
		arc::vec2 PCCTop = arc::Math::InterpF<Linear>(TLPP, TRPP, float(PlayerCC->x) / 32);
		arc::vec2 PCCBottom = arc::Math::InterpF<Linear>(BLPP, BRPP, float(PlayerCC->x) / 32);
		cachedLocs.push_back(std::make_shared<arc::vec2>(
			arc::Math::InterpF<Linear>(PCCBottom, PCCTop, float(PlayerCC->y) / 32)
		));
	}
	for (auto& rock : m_Rocks)
	{
		auto dist1 = arc::vec2::Dist(rock->Sprite.get_location(), m_Player->PlayerSprite.get_location());
		auto dist2 = arc::Math::Max(rock->MinDist, m_Player->MinDist);
		if (dist1 < dist2)
		{
			arc::vec2 TLRP = arc::HPR::QUAD::get_corner<TopLeft>(rock->Sprite.get_transform());
			arc::vec2 TRRP = arc::HPR::QUAD::get_corner<TopRight>(rock->Sprite.get_transform());
			arc::vec2 BLRP = arc::HPR::QUAD::get_corner<BottomLeft>(rock->Sprite.get_transform());
			arc::vec2 BRRP = arc::HPR::QUAD::get_corner<BottomRight>(rock->Sprite.get_transform());

			for (auto& RockCC : arc::rock::SpriteCollisionData[rock->SpriteIndex])
			{
				arc::vec2 RCCTop = arc::Math::InterpF<Linear>(TLRP, TRRP, float(RockCC->x) / 32);
				arc::vec2 RCCBottom = arc::Math::InterpF<Linear>(BLRP, BRRP, float(RockCC->x) / 32);

				arc::vec2 RCCN = arc::Math::InterpF<Linear>(RCCBottom, RCCTop, float(RockCC->y) / 32);

				for (auto& PlayerCC : cachedLocs)
				{
					if (arc::vec2::AlmostEqual(*PlayerCC, RCCN, 0.01f))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

void space_war::on_update(float delta_time)
{
	ARC_PROFILE_FUNCTION();
	
	static float TimePassed = 0;
	static float PrevSpawnTime = 0;
	TimePassed += delta_time;
	if ((int)(TimePassed*10) % 10>8)
		m_Blink = !m_Blink;

	m_CameraController.on_update(delta_time);

	arc::render_command::set_clear_colour({ .1f, .1f, .1f, 1.f });
	arc::render_command::clear();
	
	arc::CRenderer2D::begin_scene(m_CameraController.get_camera());
	arc::CRenderer2D::draw_quad({0.f, 0.f}, 0.f, {10.f, 10.f}, 0.f, arc::CColor(0.8f, 0.7f, 0.3f, 1.f));

	if (GameState == game_state::Play)
	{
		if (TimePassed - PrevSpawnTime > 3) {
			if (!m_Rocks.empty() && m_Rocks.size() % 3 == 0)
			{
				m_Rocks.push_back(arc::create_ref<arc::rock>());
				i += 0.0001f;
				m_Rocks.back()->Sprite.Transform.ZOrder = i;
				m_Rocks.back()->calculate_random_position();
			
				m_Rocks.back()->Load({ 0,0 }, { 0,0 });
				m_Rocks.back()->calculate_random_position();
				m_Rocks.back()->Velocity = (m_Player->PlayerSprite.get_location() / m_Player->PlayerSprite.get_location().Length())*-1.f;
			}
			else
			{
				m_Rocks.push_back(arc::create_ref<arc::rock>());
				i += 0.01f;
				m_Rocks.back()->Sprite.Transform.ZOrder = i;
				m_Rocks.back()->Load({ 0,0 }, { 0,0 });
				m_Rocks.back()->calculate_random_position();
				m_Rocks.back()->calculate_random_velocity();
			}
			PrevSpawnTime = TimePassed;
		}
		if (detect_collision())
		{
			GameState = game_state::GameOver;
			m_Blink=true;
		}
		m_Player->on_update(delta_time);
		for (auto& rock : m_Rocks) {
			rock->on_update(delta_time);
		}
	}
	arc::CRenderer2D::draw_quad(m_Player->PlayerSprite);
	m_Player->ParticleSystem.OnRender();
	for (auto& rock : m_Rocks) {
		arc::CRenderer2D::draw_quad(rock->Sprite);
	}
	arc::CRenderer2D::end_scene();
}

void space_war::on_event(arc::event& event)
{
	ARC_PROFILE_FUNCTION();
	m_Player->on_event(event);
	m_CameraController.on_event(event);

	arc::event_dispatcher dispatcher(event);
	dispatcher.Dispatch<arc::mouse_button_pressed_event>(BIND_FN(&space_war::on_mouse_button_pressed));
}

bool space_war::on_mouse_button_pressed(arc::mouse_button_pressed_event& _e)
{
	if (GameState == game_state::GameOver)
		reset_level();

	GameState = game_state::Play;
	return false;
}

void space_war::on_gui_render()
{
	ARC_PROFILE_FUNCTION();


	switch (GameState)
	{
		case game_state::Play:
		{
			uint32_t playerScore = (uint32_t)m_Rocks.size();
			std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
			ImGui::get_foreground_draw_list()->AddText(m_Font, 48.0f, ImGui::get_window_pos(), 0xffffffff, scoreStr.c_str());
			break;
		}
		case game_state::MainMenu:
		{
			auto pos = ImGui::get_window_pos();
			auto width = arc::application::Get().get_window().get_width();
			auto height = arc::application::Get().get_window().get_height();
			pos.x += width * 0.5f - 460.0f;
			pos.y += 50.0f;
			if (m_Blink)
				ImGui::get_foreground_draw_list()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");
			break;
		}
		case game_state::GameOver:
		{
			auto pos = ImGui::get_window_pos();
			auto width = arc::application::Get().get_window().get_width();
			auto height = arc::application::Get().get_window().get_height();
			pos.x += width * 0.5f - 460.0f;
			pos.y += 50.0f;
			if (m_Blink)
				ImGui::get_foreground_draw_list()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");

			pos.x += 305.0f;
			pos.y += 150.0f;
			uint32_t playerScore = (uint32_t)m_Rocks.size();
			std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
			ImGui::get_foreground_draw_list()->AddText(m_Font, 48.0f, pos, 0xffffffff, scoreStr.c_str());
			break;
		}
	}
}

void space_war::reset_level()
{
	GameState = game_state::Play;
	i=0.5;
	m_Player->Reset();
	m_Rocks.clear();
}
