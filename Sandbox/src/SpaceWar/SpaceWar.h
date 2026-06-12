#pragma once
#include "ARC/Core/Application.h"
#include "ARC/Renderer/OrthographicCameraController.h"
#include "ARC/Objects/Primitive2D.h"
#include "ARC/Renderer/Color.h"
#include "ARC/Renderer/Layer.h"
#include <vector>
#include "imgui/imgui.h"

namespace arc { class mouse_button_pressed_event; }
namespace arc { class vertex_array; }
namespace arc { class texture_2d; }
namespace arc { class player; }
namespace arc { class shader; }
namespace arc { class event; }
namespace arc { class rock; }

enum class game_state {
	Play = 0,
	MainMenu = 1,
	GameOver = 2
};

class space_war : public arc::layer
{
public:
	space_war();
	virtual ~space_war() = default;

	virtual void on_attach();
	virtual void on_detach();

	virtual void on_update(float delta_time) override;
	virtual void on_event(arc::event& event) override;
	virtual void on_gui_render() override;
	
	void reset_level();
	bool detect_collision();
	
	bool on_mouse_button_pressed(arc::mouse_button_pressed_event& _e);

	float prev = 0.f;
	game_state GameState;
protected:
	arc::ref<arc::shader> m_Shader;
	arc::ref<arc::vertex_array> m_VertexArray;
	arc::ref<arc::player> m_Player;
	//particle_system_2d m_ParticleSystem;
	std::vector<arc::ref<arc::rock>> m_Rocks;

	bool m_Blink = true; 
	ImFont* m_Font;

private:
	arc::orthographic_camera_controller m_CameraController;
};