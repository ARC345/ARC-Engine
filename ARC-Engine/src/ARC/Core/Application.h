#pragma once

#include "Core.h"

#include "ARC/Renderer/layer_stack.h"

#include <string>
#include <memory>
#include "../Types/vector.h"

namespace arc { class layer; }
namespace arc { class imgui_layer; }

namespace arc { class window_close_event; }
namespace arc { class window_resize_event; }
namespace arc { class event; }

namespace arc{
	class window;
	class ARC_API application
	{
	public:
		application(const string& name = "ARC-Engine");
		virtual ~application();

		void run();
		void on_event(event& _e);

		void push_layer(layer* _layer);
		void push_overlay(layer* _overlay);

		inline window& get_window() const { return *window_; }

		void shutdown();
		imgui_layer* get_imgui_layer() { return imgui_layer_; };

		inline static application& Get() { return *instance; }
		inline virtual string get_app_name() =0;
		inline const string& get_engine_name() { static string rval = "ARC_Engine"; return rval; };
		inline float get_delta_time() const { return delta_time_; }

	private:
		bool on_window_close(window_close_event& pE);
		bool on_window_resize(window_resize_event& pE);
		
	private:
		
		float delta_time_;
		float last_frame_time_ = 0;
		
		std::unique_ptr<window> window_;
		
		imgui_layer* imgui_layer_;
		u8 running_ : 1;
		u8 minimized_: 1;
		layer_stack layer_stack_;
		static application* instance;
	};

	//Defined in client
	application* create_application();
}