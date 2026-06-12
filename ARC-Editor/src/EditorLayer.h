#pragma once

#include "ARC/Renderer/Layer.h"
#include "ARC/Renderer/OrthographicCameraController.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "ARC/Scene/EditorCamera.h"
#include "ARC/Core/Macros.h"

namespace arc { class life_sim_2d; }
namespace arc { class framebuffer; }
namespace arc { class scene; }
namespace arc { class key_pressed_event; }
namespace arc { class mouse_button_pressed_event; }
namespace arc { class mouse_button_released_event; }

namespace arc {
	enum class scene_state : u8 {
		None = 0,
		Edit=1,
		Play=2,
		Simulate=3
	};

	class editor_layer : public layer
	{
	public:
		editor_layer();
		virtual ~editor_layer() = default;
		virtual void on_gui_render() override;

		virtual void on_attach() override;
		virtual void on_detach() override;

		virtual void on_update(float pDeltaTime) override;
		virtual void on_event(event& pEvent) override;

		bool on_key_pressed(key_pressed_event& pE);
		bool OnMousePressedEvent(mouse_button_pressed_event& pE);

		void new_scene();
		void open_scene();
		void open_scene(const std::filesystem::path& pFilepath);
		void save_scene_as();
		void save_scene();

		void set_scene_state(scene_state pNewState);

		void OnOverlayRender();

	public:

	#ifdef ARC_DRAW_DEBUG_SHAPES
		bool DrawDebugShapes;
		float DebugCircleColliderThickness = 0.1f;
		color4 DebugCircleColliderColor = color4::Green();
		float DebugBoxColliderThickness = 0.1f;
		color4 DebugBoxColliderColor = color4::Green();
	#endif
		
	protected:
	private:
		u8 is_viewport_focused_ : 1;
		u8 is_viewport_hovered_ : 1;
		vec2 viewport_size_;
		vec2 viewport_min_bound_;
		vec2 viewport_max_bound_;

		int guizmo_type_ = -1;
		entity hovered_entity_;
		ref<framebuffer> frame_buffer_;
		ref<scene> active_scene_;
		ref<scene> editor_scene_;
		ref<life_sim_2d> life_sim2_d_;
		
		std::filesystem::path editor_scene_path_;

		editor_camera editor_camera_;
		orthographic_camera_controller camera_controller_;
		scene_hierarchy_panel scene_hierachy_panel_;
		content_browser_panel content_browser_panel_;

		scene_state scene_state_ = scene_state::Edit;
		ref<texture_2d> play_button_texture_;
		ref<texture_2d> stop_button_texture_;
		ref<texture_2d> simulate_button_texture_;
		ref<texture_2d> pause_button_texture_;
	};
}