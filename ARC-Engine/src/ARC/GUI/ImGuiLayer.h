#pragma once
#include "ARC\Renderer\Layer.h"

namespace arc {
	class ARC_API imgui_layer : public layer
	{
	public:
		imgui_layer();
		~imgui_layer();

		virtual void on_attach() override;
		virtual void on_detach() override;
		virtual void on_gui_render() override;
		virtual void on_event(event& _e) override;

		void begin();
		void end();

		void set_block_events(bool _) {block_events_ = _; };
		void set_dark_theme_colors();
	private:
		uint8_t block_events_ : 1;
		float time_ = 0.f;
	};
}