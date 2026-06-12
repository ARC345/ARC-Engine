#pragma once
#include "ARC\Types\Pointer.h"
#include "ARC\Scene\Entity.h"

namespace arc { class scene; }

namespace arc {
	class scene_hierarchy_panel
	{
	public:
		scene_hierarchy_panel()=default;
		scene_hierarchy_panel(const ref<scene>& pScene);

		void set_context(const ref<scene>& pScene);
		void on_imgui_render();

		entity get_selected_entity() const { return selected_entity_; }
		void set_selected_entity(entity p) { selected_entity_ = p; }

	private:
		void DrawEntityNode(entity pEntity);

	private:
		ref<scene> context_;
		entity selected_entity_;
	};
}