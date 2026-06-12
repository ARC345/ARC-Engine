#include "PCH/arc_pch.h"
#include "EditorLayer.h"
#include "imgui/imgui.h"
#include "ARC/Objects/Primitive2D.h"
#include "ARC/Renderer/Renderer2D.h"
#include "ARC/Renderer/RenderCommand.h"
#include "ARC/Scene/Scene.h"
#include "ARC/Scene/Entity.h"
#include "ARC/Renderer/FrameBuffer.h"
#include "LifeSim2D/LifeSim2D.h"
#include "ImGuizmo.h"
#include "glm/gtc/type_ptr.inl"
#include "glm\gtx\transform.hpp"
#include "ARC/Wrappers/Glm.h"
#include "ARC/Scene/BasicComponents.h"
#include "ARC/Events/MouseEvent.h"
#include "ARC/Events/KeyEvent.h"

namespace arc {
	static entity ESquare;
	static entity ECamera;

	editor_layer::editor_layer() :
		layer("EditorLayer"),
		camera_controller_(1280.f / 780.f, true)
		#ifdef ARC_DRAW_DEBUG_SHAPES
		, DrawDebugShapes(1u)
		#endif // ARC_DRAW_DEBUG_SHAPES
		
	{
	}

	void editor_layer::on_attach()
	{
		play_button_texture_ = texture_2d::create(std::filesystem::path("resources/icons/PlayButton.png"));
		stop_button_texture_ = texture_2d::create(std::filesystem::path("resources/icons/StopButton.png"));
		pause_button_texture_ = texture_2d::create(std::filesystem::path("resources/icons/PauseButton.png"));
		simulate_button_texture_ = texture_2d::create(std::filesystem::path("resources/icons/SimulateButton.png"));

		frame_buffer_specification frame_buffer_specs;
		frame_buffer_specs.Width = 1280;
		frame_buffer_specs.Height = 720;
		frame_buffer_specs.Attachments = { frame_buffer_texture_format::RGBA8, frame_buffer_texture_format::RED_INTEGER, frame_buffer_texture_format::DEPTH24STENCIL8 };
		frame_buffer_ = framebuffer::create(frame_buffer_specs);

		active_scene_ = create_ref<scene>();

		editor_scene_ = active_scene_;

		life_sim2_d_ = create_ref<life_sim_2d>();
		scene_hierachy_panel_.set_context(active_scene_);
		editor_camera_ = editor_camera(30.0f, 1.778f, 0.1f, 1000.0f);

		life_sim2_d_->on_attach(active_scene_);
	}

	void editor_layer::on_detach()
	{
		life_sim2_d_->on_detach();
	}

	void editor_layer::on_update(float delta_time)
	{
		if (frame_buffer_specification spec = frame_buffer_->get_specifications();
			viewport_size_.x > 0.0f && viewport_size_.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != viewport_size_.x || spec.Height != viewport_size_.y))
		{
			frame_buffer_->resize({ (u32)viewport_size_.x, (u32)viewport_size_.y });
			camera_controller_.OnResize(viewport_size_.x, viewport_size_.y);
			editor_camera_.set_viewport_size(viewport_size_.x, viewport_size_.y);
			active_scene_->on_viewport_resize({ (u32)viewport_size_.x, (u32)viewport_size_.y });
		}

		if (is_viewport_hovered_ && is_viewport_focused_)	camera_controller_.on_update(delta_time);
		
		renderer_2d::ResetStats();

		frame_buffer_->bind();

		render_command::set_clear_colour({ .1f, .1f, .1f, 1.f });
		render_command::clear();

		frame_buffer_->ClearColorAttachment(1, -1);

		switch (scene_state_) {
		case scene_state::Edit:
			active_scene_->on_update_editor(delta_time, editor_camera_);
			life_sim2_d_->on_update_editor(delta_time);
			break;
		case scene_state::Play:
			active_scene_->on_update_runtime(delta_time);
			life_sim2_d_->on_update_runtime(delta_time);
			break;
		case scene_state::Simulate:
			active_scene_->on_update_simulation(delta_time, editor_camera_);
			break;
		}

		auto viewportSize = viewport_max_bound_-viewport_min_bound_;
		auto MousePos = (vec2&)ImGui::get_mouse_pos() - viewport_min_bound_;
		MousePos.y = viewportSize.y - MousePos.y;

		if (MousePos.x >= 0 && MousePos.y >= 0 && MousePos.x < viewportSize.x && MousePos.y < viewportSize.y) {
			auto readPixel = frame_buffer_->read_pixel(1, MousePos.x, MousePos.y);
			hovered_entity_ =  readPixel == -1 ? entity{} : entity{ entity_id(readPixel), active_scene_.get() };
		}
		OnOverlayRender();
		frame_buffer_->UnBind();

		editor_camera_.on_update(delta_time);
	}

	void editor_layer::on_event(event& event)
	{
		editor_camera_.on_event(event);
		event_dispatcher dispatcher(event);
		dispatcher.Dispatch<key_pressed_event>(BIND_FN(&editor_layer::on_key_pressed));
		dispatcher.Dispatch<mouse_button_pressed_event>(BIND_FN(&editor_layer::OnMousePressedEvent));
	}
	bool editor_layer::on_key_pressed(key_pressed_event& pE)
	{
		if (pE.get_repeat_count() > 0)
			return false;

		bool control = input::is_key_pressed(key::LeftControl) || input::is_key_pressed(key::RightControl);
		bool shift = input::is_key_pressed(key::LeftShift) || input::is_key_pressed(key::RightShift);

		switch (pE.get_key_code())
		{
		// Scene
		case key::N:
		{
			if (control)
				new_scene();

			break;
		}
		case key::O:
		{
			if (control)
				open_scene();

			break;
		}
		case key::S:
		{
			if (control) {
				if (shift)
					save_scene_as();
				else
					save_scene();
			}
			break;
		}
		// Editor
		case key::D:
		{
			switch (scene_state_) {
			case scene_state::Edit:
				if (control && scene_hierachy_panel_.get_selected_entity()) 
					scene_hierachy_panel_.get_selected_entity().Duplicate(); // @TODO: Rename
				
				break;
			};
			break;
		}
		
		// Gizmos
		case key::Q:
		{
			if (!ImGuizmo::is_using())
				guizmo_type_ = -1;
			break;
		}
		case key::W:
		{
			if (!ImGuizmo::is_using())
				guizmo_type_ = ImGuizmo::OPERATION::TRANSLATE;
			break;
		}
		case key::E:
		{
			if (!ImGuizmo::is_using())
				guizmo_type_ = ImGuizmo::OPERATION::ROTATE;
			break;
		}
		case key::R:
		{
			if (!ImGuizmo::is_using())
				guizmo_type_ = ImGuizmo::OPERATION::SCALE;
			break;
		}
		}
	}
	bool editor_layer::OnMousePressedEvent(mouse_button_pressed_event& pE)
	{
		if (pE.get_mouse_button() == mouse::ButtonLeft)
			if (is_viewport_hovered_ && !ImGuizmo::is_over() && !input::is_key_pressed(key::LeftAlt))
				scene_hierachy_panel_.set_selected_entity(hovered_entity_);
		return false;
	}

	void editor_layer::on_gui_render()
	{
		ARC_PROFILE_FUNCTION();
		// Note: Switch this to true to enable dockspace
		static bool dockingEnabled = true;
		if (dockingEnabled)
		{
			static bool dockspaceOpen = true;
			static bool opt_fullscreen_persistant = true;
			bool opt_fullscreen = opt_fullscreen_persistant;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen)
			{
				ImGuiViewport* viewport = ImGui::get_main_viewport();
				ImGui::set_next_window_pos(viewport->Pos);
				ImGui::set_next_window_size(viewport->size);
				ImGui::set_next_window_viewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}

			// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask begin() to not render a background.
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			// Important: note that we proceed even if begin() returns false (aka window is collapsed).
			// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
			// all active windows docked into it will lose their parent and become undocked.
			// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
			// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::begin("DockSpace Demo", &dockspaceOpen, window_flags);
			ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			// DockSpace
			ImGuiIO& io = ImGui::get_io();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::get_id("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					// Disabling fullscreen would allow the window to be moved to the front of other windows, 
					// which we can't undo at the moment without finer window depth/z control.

					if (ImGui::MenuItem("New")) {}
					if (ImGui::MenuItem("Save")) {}
					if (ImGui::MenuItem("Open...", "Ctrl+O"))
					{
						auto filepath = file_dialogs::open_file("ARC-Engine Scene (*.arc)\0*.arc\0");
						if (!filepath.empty())
							open_scene(filepath);
					}
					if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					{
						auto filepath = file_dialogs::save_file("ARC-Engine Scene (*.arc)\0*.arc\0");
						if (!filepath.empty())
							active_scene_->serialize_to_text(filepath);
					}
					if (ImGui::MenuItem("Exit")) application::Get().shutdown();
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			scene_hierachy_panel_.on_imgui_render();
			content_browser_panel_.on_imgui_render();

			ImGui::begin("set_tings");

			auto stats = renderer_2d::get_stats();
			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.get_total_vertex_count());
			ImGui::Text("Indices: %d", stats.get_total_index_count());

			string name = "None";

			if (hovered_entity_ && hovered_entity_.has_component<name_component>())
				name = hovered_entity_.get_component<name_component>().Name;

			ImGui::Text("Hovered Entity: %s", name.c_str());

			if (ImGui::TreeNode("Debug"))
			{
				if (ImGui::TreeNode("Colliders"))
				{
					ImGui::Checkbox("Draw Debug Colliders", &DrawDebugShapes);
					ImGui::Separator();


					ImGui::ColorEdit4("Circle Line Color", DebugCircleColliderColor.Data());
					ImGui::DragFloat("Circle Line Thickness", &DebugCircleColliderThickness);
					ImGui::Separator();

					ImGui::ColorEdit4("Box Line Color", DebugBoxColliderColor.Data());
					ImGui::DragFloat("Box Line Thickness", &DebugBoxColliderThickness);

					ImGui::TreePop();
				}
				ImGui::TreePop();
			}

			ImGui::end();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 5, 5 });
			ImGui::begin("Viewport");
			auto viewportMinRegion = (vec2&)ImGui::get_window_content_region_min();
			auto viewportMaxRegion = (vec2&)ImGui::get_window_content_region_max();
			auto viewportOffset = (vec2&)ImGui::get_window_pos();

			viewport_min_bound_ = viewportMinRegion + viewportOffset;
			viewport_max_bound_ = viewportMaxRegion + viewportOffset;

			is_viewport_focused_ = ImGui::is_window_focused();
			is_viewport_hovered_ = ImGui::is_window_hovered();
			
			application::Get().get_imgui_layer()->set_block_events(!is_viewport_focused_ || !is_viewport_hovered_);
			ImVec2 viewportPanelSize = ImGui::get_content_region_avail();
			viewport_size_ = { viewportPanelSize.x, viewportPanelSize.y };
			u64 textureID = frame_buffer_->get_color_attachment_renderer_id();
			
			ImGui::Image((void*)textureID, ImVec2{ viewport_size_.x, viewport_size_.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			if (ImGui::BeginDragDropTarget())
			{
				if (const auto* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const auto* path = (const wchar_t*)payload->Data;
					open_scene(std::filesystem::path("assets")/std::filesystem::path(path));
				}
				ImGui::EndDragDropTarget();
			}

			// Gizmos
			entity selectedEntity = scene_hierachy_panel_.get_selected_entity();
			if (selectedEntity && guizmo_type_ != -1)
			{
				ImGuizmo::set_orthographic(false);
				ImGuizmo::set_drawlist();
				ImGuizmo::set_rect(viewport_min_bound_.x, viewport_min_bound_.y, viewport_max_bound_.x-viewport_min_bound_.x, viewport_max_bound_.y - viewport_min_bound_.y);
				
				if (selectedEntity.has_component<transform_2d_component>())
				{
					// Snapping
					bool snap = input::is_key_pressed(key::LeftControl);
					float snapValue = 0.5f; // Snap to 0.5m for translation/scale

					// Snap to 45 degrees for rotation
					if (guizmo_type_ == ImGuizmo::OPERATION::ROTATE)
						snapValue = 45.0f;

					float snapValues[3] = { snapValue, snapValue, snapValue };

					auto& entityTransform = selectedEntity.get_component<transform_2d_component>().Transform;
					auto entityTransformMatrix = convert<mat4, transform_2d>::Conv(entityTransform);
					ImGuizmo::Manipulate(
						glm::value_ptr(editor_camera_.get_view_matrix()),
						glm::value_ptr(editor_camera_.get_projection()),
						(ImGuizmo::OPERATION)guizmo_type_,
						ImGuizmo::LOCAL,
						glm::value_ptr(entityTransformMatrix),
						nullptr,
						snap ? snapValues : nullptr
					);
					if (ImGuizmo::is_using())
						entityTransform = convert<transform_2d, mat4>::Conv(entityTransformMatrix);
				}
			}

			ImGui::end();
			ImGui::PopStyleVar();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 2));
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0.1));
			const auto& tmp = ImGui::get_style().Colors[ImGuiCol_ButtonHovered];
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { tmp.x,tmp.y,tmp.z,tmp.w / 2 });
			const auto& tmp2 = ImGui::get_style().Colors[ImGuiCol_ButtonHovered];
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { tmp2.x,tmp2.y,tmp2.z,tmp2.w / 2 });
			ImGui::begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse);
			auto buttonSize = ImGui::get_window_height()-10;
			switch (scene_state_) {
			case scene_state::Edit:
				ImGui::set_cursor_pos_x((ImGui::get_content_region_max().x) * 0.5f - buttonSize);

				if (ImGui::ImageButton((ImTextureID)(uint64_t)play_button_texture_->get_renderer_id(), ImVec2(buttonSize, buttonSize), ImVec2(0, 0), ImVec2(1, 1), 0)) {
					set_scene_state(scene_state::Play);
				}
				ImGui::SameLine();
				if (ImGui::ImageButton((ImTextureID)(uint64_t)simulate_button_texture_->get_renderer_id(), ImVec2(buttonSize, buttonSize), ImVec2(0, 0), ImVec2(1, 1), 0)) {
					set_scene_state(scene_state::Simulate);
				}
				break;
			case scene_state::Play: case scene_state::Simulate:
				ImGui::set_cursor_pos_x((ImGui::get_content_region_max().x - buttonSize) * 0.5f);
				if (ImGui::ImageButton((ImTextureID)(uint64_t)stop_button_texture_->get_renderer_id(), ImVec2(buttonSize, buttonSize), ImVec2(0, 0), ImVec2(1, 1), 0)) {
					set_scene_state(scene_state::Edit);
				}
				break;
			}

			ImGui::end();
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(3);
			life_sim2_d_->on_gui_render();
			ImGui::end();

		}
	}

	void editor_layer::new_scene()
	{
		active_scene_ = create_ref<scene>();
		active_scene_->on_viewport_resize(vec2<uint32_t>(viewport_size_.x, viewport_size_.y));
		scene_hierachy_panel_.set_context(active_scene_);
		editor_scene_path_ = std::filesystem::path();
	}

	void editor_layer::open_scene()
	{
		open_scene(file_dialogs::open_file("ARC-Engine Scene (*.arc)\0*.arc\0"));
	}

	void editor_layer::open_scene(const std::filesystem::path& pFilepath)
	{
		if (pFilepath.empty()) return;
		if (pFilepath.extension().string() != ".arc") return;

		editor_scene_ = create_ref<scene>();
		editor_scene_->on_viewport_resize(vec2<uint32_t>(viewport_size_.x, viewport_size_.y));
		editor_scene_->deserialize_from_text(pFilepath);

		set_scene_state(scene_state::Edit);

		editor_scene_path_ = pFilepath;
	}

	void editor_layer::save_scene_as()
	{
		auto filepath = file_dialogs::save_file("ARC-Engine Scene (*.arc)\0*.arc\0");
		if (filepath.empty()) return;
		editor_scene_path_ = filepath;
		active_scene_->serialize_to_text(filepath);
	}

	void editor_layer::save_scene()
	{
		if (editor_scene_path_.empty()) save_scene_as();
		else active_scene_->serialize_to_text(editor_scene_path_);
	}

	void editor_layer::set_scene_state(scene_state pNewState)
	{
		if (scene_state_ == scene_state::Play || scene_state_ == scene_state::Simulate) {
			active_scene_->on_set_scene_state((u8)scene_state_, 0);
		}

		active_scene_ = pNewState == scene_state::Edit ? editor_scene_ : scene::copy(editor_scene_); 
		
		life_sim2_d_->set_scene(active_scene_);
		active_scene_->on_set_scene_state(0, (u8)pNewState);
		scene_state_ = pNewState;

		scene_hierachy_panel_.set_context(active_scene_);
	}

	void editor_layer::OnOverlayRender()
	{
		#ifdef ARC_DRAW_DEBUG_SHAPES
		renderer_2d::end_scene();
		switch (scene_state_) {
		case scene_state::Edit: case scene_state::Simulate:
			renderer_2d::begin_scene(editor_camera_);
			break;
		case scene_state::Play:
			if (auto e = active_scene_->get_primary_camera_entity())
				renderer_2d::begin_scene(e.get_component<camera_component>().Camera, e.get_component<transform_2d_component>().Transform);
			break;
		}
		active_scene_->FilterByComponents<transform_2d_component, circle_collider_2d_component>().each([&](auto e, auto& transformComponent, auto& circleColliderComponent) {
			if (DrawDebugShapes)
			{
				renderer_2d::draw_circle(transformComponent.Transform.Location+vec3(circleColliderComponent.Offset.x, circleColliderComponent.Offset.y, 0.001), transformComponent.Transform.Rotation, vec2(circleColliderComponent.Radius) * transformComponent.Transform.Scale.x * 2.f, DebugCircleColliderColor, DebugCircleColliderThickness, 0.995, (int)e);
			}
			});
		active_scene_->FilterByComponents<transform_2d_component, box_collider_2d_component>().each([&](auto e, auto& transformComponent, auto& boxColliderComponent) {
			if (DrawDebugShapes)
			{
				float lT = renderer_2d::get_line_thickness();
				renderer_2d::set_line_thickness(DebugBoxColliderThickness);
				renderer_2d::DrawRect(
					transformComponent.Transform.Location + vec3(boxColliderComponent.Offset.x, boxColliderComponent.Offset.y, 0.001),
					transformComponent.Transform.Rotation,
					boxColliderComponent.size * transformComponent.Transform.Scale * 2.f,
					DebugBoxColliderColor,
					(int)e);
				renderer_2d::set_line_thickness(lT);
			}
			});
		renderer_2d::end_scene();
		#endif // ARC_DRAW_DEBUG_SHAPES
		
	}
}