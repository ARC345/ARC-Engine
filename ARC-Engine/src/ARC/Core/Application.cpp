#include "arc_pch.h"
#include "Application.h"
#include "ARC/Events/ApplicationEvent.h"
#include "Window.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "ARC/Input/Input.h"
#include "ARC/Renderer/Shader.h"
#include "ARC/Renderer/Buffer.h"
#include "ARC/Renderer/VertexArray.h"
#include "ARC/Renderer/Renderer.h"
#include "ARC/GUI/ImGuiLayer.h"
#include "ARC/Helpers/Math.h"

namespace arc
{
	application* application::instance=nullptr;
	application::application(const string& name /*= "ARC-Engine"*/) :
		running_(1u)
	{
		ARC_PROFILE_FUNCTION();

		ARC_CORE_ASSERT(!instance, "Application already exists");
		
		instance= this;

		window_ = std::unique_ptr<window>(window::create(name));
			
		renderer::init();

		imgui_layer_ = new imgui_layer();
		push_overlay(imgui_layer_);
			
		window_->set_event_callback(BIND_FN(&application::on_event));
	}

	application::~application() {}

	void application::run()
	{
		ARC_PROFILE_FUNCTION();
		while (running_)
		{
			//@TEMP
			auto time = (float)glfwGetTime();
			delta_time_ = time - last_frame_time_;
			last_frame_time_ = time;

			if(!minimized_) {
				for (layer* layer : layer_stack_)
					layer->on_update(delta_time_);
			}
			imgui_layer_->begin();
			for (layer* layer : layer_stack_)
				layer->on_gui_render();
			imgui_layer_->end();
			window_->on_update();
		}
	}

	void application::on_event(event& _e)
	{
		event_dispatcher dispatcher(_e);
		dispatcher.Dispatch<window_close_event>(BIND_FN(&application::on_window_close));
		dispatcher.Dispatch<window_resize_event>(BIND_FN(&application::on_window_resize));

		for (auto it = layer_stack_.end(); it!= layer_stack_.begin();)
		{
			(*--it)->on_event(_e);
			if (_e.bHandled) break;
		}
	}

	void application::push_layer(layer* _layer)
	{
		layer_stack_.push_layer(_layer);
		_layer->on_attach();
	}
	void application::push_overlay(layer* _overlay)
	{
		layer_stack_.push_overlay(_overlay);
		_overlay->on_attach();
	}

	void application::shutdown()
	{
		running_=false;
	}

	bool application::on_window_close(window_close_event& _e)
	{
		running_ = false;
		return true;
	}
	bool application::on_window_resize(window_resize_event& _e)
	{
		if(_e.get_x() == 0 || _e.get_y() == 0){
			minimized_ = true;
			return false;
		}
		minimized_ = false;
		get_window().set_width(_e.get_x());
		get_window().set_height(_e.get_y());
		renderer::on_window_resize(vec2<uint32_t>(_e.get_x(), _e.get_y()));
		return false;
	}
}