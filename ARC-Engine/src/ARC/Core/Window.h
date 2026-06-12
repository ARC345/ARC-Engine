#pragma once

#include <functional>

namespace arc { class event; }

namespace arc {
	struct window_props {
		string Title;
		uint32_t Width;
		uint32_t Height;

		window_props(const string& _title = "ARC-Engine", uint32_t _width = 1280, uint32_t _height = 720) : Title(_title), Width(_width), Height(_height){}
	};

	class window {
	public:
		using event_callback_fn = std::function<void(event&)>;

		virtual ~window() {}
		virtual void on_update() = 0;

		virtual uint32_t get_width() const = 0;
		virtual uint32_t get_height() const = 0;
		virtual void set_width(uint32_t _) = 0;
		virtual void set_height(uint32_t _) = 0;
		
		virtual void set_event_callback(const event_callback_fn& _callback) = 0;
		virtual void set_vsync(bool _bEnabled) = 0;
		virtual bool is_vsync() const = 0;

		virtual void* get_native_window() const = 0;

		static window* create(const window_props& _props = window_props());
	};
}